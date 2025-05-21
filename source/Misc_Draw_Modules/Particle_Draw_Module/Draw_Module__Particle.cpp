#include <Misc_Draw_Modules/Particle_Draw_Module/Draw_Module__Particle.h>

#include <Math_Stuff.h>

#include <glew.h>

#include <Binds_Controller/Binds_Controller.h>

#include <Misc_Draw_Modules/Particle_Draw_Module/Particle_Graphics_Component_Reconstructors/Graphics_Component_Reconstructor__Particle.h>

using namespace LMD;


Draw_Module__Particle::Draw_Module__Particle()
{
    glGenBuffers(1, &m_element_array_buffer);
}

Draw_Module__Particle::~Draw_Module__Particle()
{
    glDeleteBuffers(1, &m_element_array_buffer);
}



void Draw_Module__Particle::set_max_particles(unsigned int _amount)
{
    L_ASSERT(_amount > 0);

    m_max_particles_amount = _amount;
    m_alive_particles.resize_and_fill(m_max_particles_amount, false);

    for(LR::Draw_Module::Graphics_Component_List::Const_Iterator it = graphics_components().begin(); !it.end_reached(); ++it)
    {
        LR::Graphics_Component* base_component_ptr = *it;
        L_ASSERT(LV::cast_variable<LR::Graphics_Component__Default>(base_component_ptr));
        LR::Graphics_Component__Default* component = (LR::Graphics_Component__Default*)base_component_ptr;
        L_ASSERT(LV::cast_variable<Graphics_Component_Reconstructor__Particle>(component->reconstructor()));
        Graphics_Component_Reconstructor__Particle* reconstructor = (Graphics_Component_Reconstructor__Particle*)component->reconstructor();

        reconstructor->set_max_particles_amount(_amount);
    }
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



void Draw_Module__Particle::create_particle(unsigned int _index)
{
    m_alive_particles[_index] = true;

    for(LR::Draw_Module::Graphics_Component_List::Const_Iterator it = graphics_components().begin(); !it.end_reached(); ++it)
    {
        LR::Graphics_Component* base_component_ptr = *it;
        LR::Graphics_Component__Default* component = (LR::Graphics_Component__Default*)base_component_ptr;
        Graphics_Component_Reconstructor__Particle* reconstructor = (Graphics_Component_Reconstructor__Particle*)component->reconstructor();

        reconstructor->create_particle(_index);
    }

    ++m_alive_particles_amount;
}

void Draw_Module__Particle::destroy_particle(unsigned int _index)
{
    m_alive_particles[_index] = false;

    for(LR::Draw_Module::Graphics_Component_List::Const_Iterator it = graphics_components().begin(); !it.end_reached(); ++it)
    {
        LR::Graphics_Component* base_component_ptr = *it;
        LR::Graphics_Component__Default* component = (LR::Graphics_Component__Default*)base_component_ptr;
        Graphics_Component_Reconstructor__Particle* reconstructor = (Graphics_Component_Reconstructor__Particle*)component->reconstructor();

        reconstructor->destroy_particle(_index);
    }

    --m_alive_particles_amount;
}

void Draw_Module__Particle::emit_particles(unsigned int _amount)
{
    L_ASSERT(_amount > 0);

    for(unsigned int i=0; i<m_alive_particles.size(); ++i)
    {
        if(m_alive_particles[i])
            continue;

        create_particle(i);

        --_amount;
        if(_amount == 0)
            break;
    }
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

    if(m_requested_particles_amount > 0)
    {
        emit_particles(m_requested_particles_amount);
        m_requested_particles_amount = 0;
    }

    Parent_Type::update(_dt);
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Draw_Module_Stub__Particle)

BUILDER_STUB_INITIALIZATION_FUNC(Draw_Module_Stub__Particle)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_emission_frequency(emission_frequency);
    product->set_max_particles(max_particles_amount);
    product->pause_emission(!autostart_emission);
}
