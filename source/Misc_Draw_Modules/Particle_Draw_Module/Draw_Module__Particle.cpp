#include <Misc_Draw_Modules/Particle_Draw_Module/Draw_Module__Particle.h>

#include <Math_Stuff.h>

#include <Misc_Draw_Modules/Particle_Draw_Module/Particle_Graphics_Component_Reconstructors/Graphics_Component_Reconstructor__Particle.h>

using namespace LMD;


void Draw_Module__Particle::set_max_particles(unsigned int _amount)
{
    m_particle_lifetimes.resize(_amount);

    for(LR::Draw_Module::Graphics_Component_List::Iterator it = graphics_components().begin(); !it.end_reached(); ++it)
    {
        LR::Graphics_Component* component = *it;
        L_ASSERT(LV::cast_variable<Graphics_Component_Reconstructor__Particle>(component->reconstructor()));
        Graphics_Component_Reconstructor__Particle* reconstructor = (Graphics_Component_Reconstructor__Particle*)component->reconstructor();

        reconstructor->set_max_particles_amount(_amount);
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


void Draw_Module__Particle::M_create_particle(unsigned int _index)
{
    float lifetime = LEti::Math::random_number_float(m_min_particles_lifetime, m_particles_lifetime_max_difference);

    for(LR::Draw_Module::Graphics_Component_List::Iterator it = graphics_components().begin(); !it.end_reached(); ++it)
    {
        LR::Graphics_Component* component = *it;
        Graphics_Component_Reconstructor__Particle* reconstructor = (Graphics_Component_Reconstructor__Particle*)component->reconstructor();

        reconstructor->create_particle(_index, lifetime);
    }

    m_particle_lifetimes[_index].start(lifetime);

    ++m_alive_particles_amount;
}

void Draw_Module__Particle::M_destroy_particle(unsigned int _index)
{
    for(LR::Draw_Module::Graphics_Component_List::Iterator it = graphics_components().begin(); !it.end_reached(); ++it)
    {
        LR::Graphics_Component* component = *it;
        Graphics_Component_Reconstructor__Particle* reconstructor = (Graphics_Component_Reconstructor__Particle*)component->reconstructor();

        reconstructor->destroy_particle(_index);
    }

    m_particle_lifetimes[_index].reset();

    --m_alive_particles_amount;
}



void Draw_Module__Particle::emit_particles(unsigned int _amount)
{
    L_ASSERT(_amount > 0);

    for(unsigned int i=0; i<m_particle_lifetimes.size(); ++i)
    {
        if(m_particle_lifetimes[i].is_active())
            continue;

        M_create_particle(i);

        if(_amount == 0)
            break;

        --_amount;
    }
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
    product->set_particles_lifetime(min_particles_lifetime, max_particles_lifetime);
    product->set_max_particles(max_particles_amount);
}
