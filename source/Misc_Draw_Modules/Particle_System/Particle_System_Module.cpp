#include <Misc_Draw_Modules/Particle_System/Particle_System_Module.h>

using namespace Particle_System;


INIT_FIELDS(Particle_System::Particle_System_Module, LV::Variable_Base)
FIELDS_END


Particle_System_Module::Particle_System_Module()
{

}

Particle_System_Module::~Particle_System_Module()
{
    delete m_particle_draw_module;
    delete m_particle_data_reseter;

    for(unsigned int i=0; i<m_particle_data.size(); ++i)
        delete m_particle_data[i];
}



void Particle_System_Module::set_transformation_data(LEti::Transformation_Data* _data)
{
    LEti::Module::set_transformation_data(_data);
    m_particle_data_reseter->set_parent_transformation_data(_data);
}

void Particle_System_Module::set_max_particles(unsigned int _amount)
{
    L_ASSERT(m_particle_data_reseter);

    for(unsigned int i=0; i<m_particle_data.size(); ++i)
        delete m_particle_data[i];

    m_particle_data.resize(_amount);

    for(unsigned int i=0; i<_amount; ++i)
        m_particle_data.push(m_particle_data_reseter->create_particle_data_instance());
}



void Particle_System_Module::forcefully_emit_particles(unsigned int _amount)
{
    for(unsigned int i=0; i<m_particle_data.size() && _amount > 0; ++i)
    {
        if(m_particle_data[i]->is_alive())
            continue;

        m_particle_data_reseter->reset_particle_data(m_particle_data[i]);
        --_amount;
    }
}



unsigned int Particle_System_Module::alive_particles_amount() const
{
    unsigned int result = 0;

    for(unsigned int i=0; i<m_particle_data.size(); ++i)
    {
        if(m_particle_data[i]->is_alive())
            ++result;
    }

    return result;
}



void Particle_System_Module::update(float _dt)
{
    L_ASSERT(m_particle_data_reseter);
    L_ASSERT(m_particle_draw_module);

    m_particle_draw_module->set_draw_on_update(false);
    m_particle_draw_module->update(_dt);
    m_particle_draw_module->set_draw_on_update(true);

    m_emission_timer.update(_dt);

    if(!m_emission_timer.is_active() && m_emission_frequency > 0.000001f)
    {
        unsigned int ratio = _dt / m_emission_frequency;
        unsigned int amount = 1;
        if(ratio > 0)
            amount = ratio;

        forcefully_emit_particles(amount);
        m_emission_timer.start(m_emission_frequency);
    }

    for(unsigned int i=0; i<m_particle_data.size(); ++i)
    {
        if(m_particle_data[i]->is_alive())
            m_particle_data[i]->update(_dt, m_particle_draw_module);
    }
}



INIT_FIELDS(Particle_System::Particle_System_Module_Stub, LEti::Module_Stub)

ADD_FIELD(unsigned int, max_particles)
ADD_FIELD(float, emission_frequency)

ADD_CHILD("particle_draw_module", particle_draw_module);
ADD_CHILD("particle_data_reseter_stub", particle_data_reseter_stub);

FIELDS_END


Particle_System_Module_Stub::~Particle_System_Module_Stub()
{
    delete particle_data_reseter_stub;
}



LV::Variable_Base* Particle_System_Module_Stub::M_construct_product() const
{
    return new Particle_System_Module;
}

void Particle_System_Module_Stub::M_init_constructed_product(LV::Variable_Base* _product) const
{
    L_ASSERT(particle_draw_module);
    L_ASSERT(particle_data_reseter_stub);

    LEti::Module_Stub::M_init_constructed_product(_product);

    Particle_System_Module* product = (Particle_System_Module*)_product;

    product->set_particle_draw_module((LR::Draw_Module*)particle_draw_module->construct());
    product->set_particle_data_reseter((Particle_Data_Reseter*)particle_data_reseter_stub->construct());
    product->set_emission_frequency(emission_frequency);

    product->set_max_particles(max_particles);
}
