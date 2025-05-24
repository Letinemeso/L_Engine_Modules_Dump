#include <Misc_Draw_Modules/Particle_Draw_Module/Draw_Module__Particle.h>

#include <Math_Stuff.h>

#include <glew.h>

#include <Binds_Controller/Binds_Controller.h>
#include <Shader/Shader_Manager.h>

using namespace LMD;


Draw_Module__Particle::Draw_Module__Particle()
{

}

Draw_Module__Particle::~Draw_Module__Particle()
{

}



void Draw_Module__Particle::set_transformation_data(LEti::Transformation_Data* _data)
{
    Parent_Type::set_transformation_data(&m_initial_transformation);
    if(_data)
    {
        m_initial_transformation.set_rotation(_data->rotation());
        m_initial_transformation.set_scale(_data->scale());
    }
    m_parent_transformation_data = _data;
}

void Draw_Module__Particle::set_transformation_data_prev_state(const LEti::Transformation_Data* _data)
{
    Parent_Type::set_transformation_data_prev_state(&m_initial_transformation);
    m_parent_transformation_data_prev_state = _data;
}



void Draw_Module__Particle::request_particles(unsigned int _amount)
{
    L_ASSERT(_amount > 0);

    m_requested_particles_amount += _amount;
}



void Draw_Module__Particle::M_update_emission_timer(float _dt)
{
    m_emission_timer.update(_dt);

    if( ! (!m_emission_timer.is_active() && m_emission_frequency > 0.000001f && !m_emission_is_paused))
        return;

    unsigned int ratio = _dt / m_emission_frequency;
    m_requested_particles_amount = 1;
    if(ratio > 0)
        m_requested_particles_amount = ratio;

    m_emission_timer.start(m_emission_frequency);
}



void Draw_Module__Particle::update(float _dt)
{
    M_update_emission_timer(_dt);

    Parent_Type::update(_dt);

    if(m_requested_particles_amount > 0)
    {
        set_compute_shader_program(m_initialization_shader);
        M_dispatch_compute_shader_if_any();
        set_compute_shader_program(m_update_shader);
        m_requested_particles_amount = 0;
    }
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Draw_Module_Stub__Particle)

BUILDER_STUB_INITIALIZATION_FUNC(Draw_Module_Stub__Particle)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_emission_frequency(emission_frequency);
    product->set_max_particles(max_particles_amount);
    product->pause_emission(!autostart_emission);

    product->set_initialization_shader(shader_manager->get_shader_program(particle_initialization_compute_shader));
    product->set_update_shader(shader_manager->get_shader_program(particle_update_compute_shader));
}
