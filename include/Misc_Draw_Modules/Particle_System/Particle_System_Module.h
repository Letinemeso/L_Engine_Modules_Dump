#pragma once

#include <Data_Structures/Vector.h>
#include <Stuff/Timer.h>

#include <Variable_Base.h>
#include <Builder_Stub.h>

#include <Module.h>

#include <Draw_Modules/Draw_Module.h>

#include <Misc_Draw_Modules/Particle_System/Particle_Data.h>
#include <Misc_Draw_Modules/Particle_System/Particle_Data_Reseter.h>


namespace Particle_System
{

    class Particle_System_Module : public LEti::Module
    {
    public:
        INIT_VARIABLE(Particle_System::Particle_System_Module, LEti::Module)

    private:
        LDS::Vector<LR::Draw_Module*> m_particle_draw_modules;
        Particle_Data_Reseter* m_particle_data_reseter = nullptr;

        LDS::Vector<Particle_Data*> m_particle_data;

        float m_emission_frequency = 0.0f;
        LST::Timer m_emission_timer;
        bool m_emission_is_paused = false;

        unsigned int m_requested_particles_amount = 0;

    public:
        Particle_System_Module();
        ~Particle_System_Module();

    public:
        inline void set_particle_data_reseter(Particle_Data_Reseter* _ptr) { m_particle_data_reseter = _ptr; }
        inline void set_emission_frequency(float _value) { m_emission_frequency = _value; }
        inline void pause_emission(bool _should_be_paused) { m_emission_is_paused = _should_be_paused; }

    public:
        inline Particle_Data_Reseter* particle_data_reseter() { return m_particle_data_reseter; }
        inline const Particle_Data_Reseter* particle_data_reseter() const { return m_particle_data_reseter; }
        inline float emission_frequency() const { return m_emission_frequency; }

    private:
        bool M_draw_module_registred(LR::Draw_Module* _ptr) const;

    public:
        void set_transformation_data(LEti::Transformation_Data* _data) override;
        void set_max_particles(unsigned int _amount);
        void add_particle_draw_module(LR::Draw_Module* _ptr);

    public:
        void forcefully_emit_particles(unsigned int _amount);
        void emit_particles(unsigned int _amount);
        void destroy_particles();

    public:
        unsigned int alive_particles_amount() const;

    private:
        void M_draw_with_particle(float _dt, LR::Draw_Module* _particle_draw_module);

    public:
        void update(float _dt) override;

    };


    class Particle_System_Module_Stub : public LEti::Module_Stub
    {
    public:
        INIT_VARIABLE(Particle_System::Particle_System_Module_Stub, LEti::Module_Stub)

        INIT_FIELDS
        ADD_FIELD(unsigned int, max_particles)
        ADD_FIELD(unsigned int, initial_particles)
        ADD_FIELD(float, emission_frequency)
        ADD_FIELD(bool, autostart_emission)
        FIELDS_END

        INIT_CHILDS
        ADD_CHILD("particle_data_reseter_stub", particle_data_reseter_stub);
        CHILDS_END

        INIT_CHILDS_LISTS
        ADD_CHILDS_LIST("Particle_Draw_Module__*", &particle_draw_modules)
        CHILDS_LISTS_END

    public:
        unsigned int max_particles = 0;
        unsigned int initial_particles = 0;
        float emission_frequency = 0.0f;
        bool autostart_emission = true;

        LV::Variable_Base::Childs_List particle_draw_modules;
        Particle_Data_Reseter_Stub* particle_data_reseter_stub = nullptr;

    public:
        ~Particle_System_Module_Stub();

    protected:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* _product) const override;

    };

}
