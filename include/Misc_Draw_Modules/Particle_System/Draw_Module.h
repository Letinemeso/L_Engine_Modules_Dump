#pragma once

#include <Data_Structures/Vector.h>
#include <Stuff/Timer.h>

#include <Variable_Base.h>
#include <Builder_Stub.h>

#include <Module.h>

#include <Draw_Modules/Draw_Module_Base.h>

#include <Misc_Draw_Modules/Particle_System/Particle_Data.h>
#include <Misc_Draw_Modules/Particle_System/Particle_Data_Reseter.h>


namespace Particle_System
{

    class Particle_System_Module : public LEti::Module
    {
    public:
        DECLARE_VARIABLE;

    private:
        LR::Draw_Module* m_particle_draw_module = nullptr;
        Particle_Data_Reseter* m_particle_data_reseter = nullptr;

        LDS::Vector<Particle_Data*> m_particle_data;

        float m_emission_frequency = 0.0f;
        LST::Timer m_emission_timer;

    public:
        Particle_System_Module();
        ~Particle_System_Module();

    public:
        inline void set_particle_draw_module(LR::Draw_Module* _ptr) { m_particle_draw_module = _ptr; }
        inline void set_particle_data_reseter(Particle_Data_Reseter* _ptr) { m_particle_data_reseter = _ptr; }
        inline void set_emission_frequency(float _value) { m_emission_frequency = _value; }

    public:
        void set_transformation_data(LEti::Transformation_Data* _data) override;
        void set_max_particles(unsigned int _amount);

    public:
        void forcefully_emit_particles(unsigned int _amount = 1);

    public:
        void update(float _dt) override;

    };


    class Particle_System_Module_Stub : public LV::Builder_Stub
    {
    public:
        DECLARE_VARIABLE;

    public:
        unsigned int max_particles = 0;
        float emission_frequency = 0.0f;

        LR::Draw_Module_Stub* particle_draw_module = nullptr;
        Particle_Data_Reseter_Stub* particle_data_reseter_stub = nullptr;

    public:
        ~Particle_System_Module_Stub();

    protected:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* _product) const override;

    };

}
