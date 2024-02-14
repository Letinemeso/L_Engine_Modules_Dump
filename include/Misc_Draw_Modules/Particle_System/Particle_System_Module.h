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
        LDS::Vector<LR::Draw_Module*> m_particle_draw_modules;
        Particle_Data_Reseter* m_particle_data_reseter = nullptr;

        LDS::Vector<Particle_Data*> m_particle_data;

        float m_emission_frequency = 0.0f;
        LST::Timer m_emission_timer;

    public:
        Particle_System_Module();
        ~Particle_System_Module();

    public:
        inline void set_particle_data_reseter(Particle_Data_Reseter* _ptr) { m_particle_data_reseter = _ptr; }
        inline void set_emission_frequency(float _value) { m_emission_frequency = _value; }

    public:
        inline Particle_Data_Reseter* particle_data_reseter() { return m_particle_data_reseter; }
        inline const Particle_Data_Reseter* particle_data_reseter() const { return m_particle_data_reseter; }

    private:
        bool M_draw_module_registred(LR::Draw_Module* _ptr) const;

    public:
        void set_transformation_data(LEti::Transformation_Data* _data) override;
        void set_max_particles(unsigned int _amount);
        void add_particle_draw_module(LR::Draw_Module* _ptr);

    public:
        void forcefully_emit_particles(unsigned int _amount = 1);

    public:
        unsigned int alive_particles_amount() const;

    public:
        void M_draw_with_particle(float _dt, LR::Draw_Module* _particle_draw_module);

    public:
        void update(float _dt) override;

    };


    class Particle_System_Module_Stub : public LEti::Module_Stub
    {
    public:
        DECLARE_VARIABLE;

    public:
        unsigned int max_particles = 0;
        float emission_frequency = 0.0f;

        LV::Variable_Base::Childs_List particle_draw_modules;
        Particle_Data_Reseter_Stub* particle_data_reseter_stub = nullptr;

    public:
        ~Particle_System_Module_Stub();

    protected:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* _product) const override;

    };

}
