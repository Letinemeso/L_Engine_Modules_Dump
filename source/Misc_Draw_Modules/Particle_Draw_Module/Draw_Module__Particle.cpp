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



void Draw_Module__Particle::add_initialization_compute_uniform_setter(LR::Uniform_Setter* _ptr)
{
    L_ASSERT(!m_initialization_uniform_setters.find(_ptr).is_ok());

    m_initialization_uniform_setters.push_back(_ptr);

    if(m_initialization_shader)
        _ptr->init(m_initialization_shader);
}



LR::Uniform_Setter* Draw_Module__Particle::get_initialization_compute_uniform_setter_with_name(const std::string& _name) const
{
    for(Uniform_Setter_List::Const_Iterator it = m_initialization_uniform_setters.begin(); !it.end_reached(); ++it)
    {
        LR::Uniform_Setter* setter = *it;
        if(setter->uniform_name() == _name)
            return setter;
    }
    return nullptr;
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

    if( ! (!m_emission_timer.is_active() && m_emission_frequency > 0.000001f && !m_emission_is_paused && _dt > 0.0f))
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
        M_dispatch_compute_shader(m_initialization_shader, m_initialization_uniform_setters);
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

    for(LV::Variable_Base::Childs_List::Const_Iterator it = initialization_compute_uniform_setter_stubs.begin(); !it.end_reached(); ++it)
    {
        L_ASSERT(LV::cast_variable<LR::Uniform_Setter_Stub>(it->child_ptr));
        LR::Uniform_Setter_Stub* stub = (LR::Uniform_Setter_Stub*)(it->child_ptr);
        product->add_initialization_compute_uniform_setter(LR::Uniform_Setter_Stub::construct_from(stub));
    }
}



Draw_Module_Stub__Particle::~Draw_Module_Stub__Particle()
{
    clear_childs_list(initialization_compute_uniform_setter_stubs);
}
