#include <Misc_Draw_Modules/Particle_System/Particle_System_Module.h>

using namespace Particle_System;


Particle_System_Module::Particle_System_Module()
{

}

Particle_System_Module::~Particle_System_Module()
{
    delete m_particle_data_reseter;

    for(unsigned int i=0; i<m_particle_draw_modules.size(); ++i)
        delete m_particle_draw_modules[i];

    for(unsigned int i=0; i<m_particle_data.size(); ++i)
        delete m_particle_data[i];
}



bool Particle_System_Module::M_draw_module_registred(LR::Draw_Module* _ptr) const
{
    for(unsigned int i=0; i<m_particle_draw_modules.size(); ++i)
    {
        if(_ptr == m_particle_draw_modules[i])
            return true;
    }
    return false;
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

void Particle_System_Module::add_particle_draw_module(LR::Draw_Module* _ptr)
{
    L_ASSERT(!M_draw_module_registred(_ptr));

    m_particle_draw_modules.push(_ptr);
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



void Particle_System_Module::M_draw_with_particle(float _dt, LR::Draw_Module* _particle_draw_module)
{
    L_ASSERT(_particle_draw_module);

    _particle_draw_module->set_draw_on_update(false);
    _particle_draw_module->update(_dt);
    _particle_draw_module->set_draw_on_update(true);

    for(unsigned int i=0; i<m_particle_data.size(); ++i)
    {
        if(m_particle_data[i]->is_alive())
            m_particle_data[i]->draw(_particle_draw_module);
    }
}



void Particle_System_Module::update(float _dt)
{
    L_ASSERT(m_particle_draw_modules.size() > 0);
    L_ASSERT(m_particle_data_reseter);

    m_emission_timer.update(_dt);

    if(!m_emission_timer.is_active() && m_emission_frequency > 0.000001f && !m_emission_is_paused)
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
            m_particle_data[i]->update(_dt);
    }

    for(unsigned int i=0; i<m_particle_draw_modules.size(); ++i)
        M_draw_with_particle(_dt, m_particle_draw_modules[i]);
}





Particle_System_Module_Stub::~Particle_System_Module_Stub()
{
    for(LV::Variable_Base::Childs_List::Iterator it = particle_draw_modules.begin(); !it.end_reached(); ++it)
        delete it->child_ptr;

    delete particle_data_reseter_stub;
}



LV::Variable_Base* Particle_System_Module_Stub::M_construct_product() const
{
    return new Particle_System_Module;
}

void Particle_System_Module_Stub::M_init_constructed_product(LV::Variable_Base* _product) const
{
    L_ASSERT(particle_draw_modules.size() > 0);
    L_ASSERT(particle_data_reseter_stub);

    LEti::Module_Stub::M_init_constructed_product(_product);

    Particle_System_Module* product = (Particle_System_Module*)_product;

    product->set_particle_data_reseter((Particle_Data_Reseter*)particle_data_reseter_stub->construct());
    product->set_emission_frequency(emission_frequency);
    product->pause_emission(!autostart_emission);

    product->set_max_particles(max_particles);

    for(LV::Variable_Base::Childs_List::Const_Iterator it = particle_draw_modules.begin(); !it.end_reached(); ++it)
    {
        LR::Draw_Module_Stub* stub = LV::cast_variable<LR::Draw_Module_Stub>(it->child_ptr);
        L_ASSERT(stub);
        product->add_particle_draw_module((LR::Draw_Module*)stub->construct());
    }
}
