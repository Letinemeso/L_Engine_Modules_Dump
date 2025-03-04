#pragma once

#include <Stuff/Timer.h>

#include <Draw_Modules/Draw_Module.h>


namespace LMD
{

    class Draw_Module__Particle : public LR::Draw_Module
    {
    public:
        INIT_VARIABLE(LMD::Draw_Module__Particle, LR::Draw_Module)

    private:
        float m_emission_frequency = 0.0f;
        bool m_emission_is_paused = false;

        LST::Timer m_emission_timer;
        unsigned int m_requested_particles_amount = 0;

        float m_min_particles_lifetime = 0.0f;
        float m_particles_lifetime_max_difference = 0.0f;
        LDS::Vector<LST::Timer> m_particle_lifetimes;
        unsigned int m_alive_particles_amount = 0;

    public:
        inline void set_emission_frequency(float _value) { m_emission_frequency = _value; }
        inline void set_particles_lifetime(float _min, float _max) { m_min_particles_lifetime = _min; m_particles_lifetime_max_difference = _max - _min; }
        inline void pause_emission(bool _value) { m_emission_is_paused = _value; }

        inline unsigned int alive_particles_amount() const { return m_alive_particles_amount; }

    public:
        void set_max_particles(unsigned int _amount);

    private:
        void M_update_emission_timer(float _dt);

        void M_create_particle(unsigned int _index);
        void M_destroy_particle(unsigned int _index);

    public:
        void emit_particles(unsigned int _amount);

    public:
        void update(float _dt) override;

    };


    class Draw_Module_Stub__Particle : public LR::Draw_Module_Stub
    {
    public:
        INIT_VARIABLE(LMD::Draw_Module_Stub__Particle, LR::Draw_Module_Stub)

        INIT_FIELDS
        ADD_FIELD(unsigned int, max_particles_amount)
        ADD_FIELD(float, emission_frequency)
        ADD_FIELD(float, min_particles_lifetime)
        ADD_FIELD(float, max_particles_lifetime)
        ADD_FIELD(bool, autostart_emission)
        FIELDS_END

        public:
        unsigned int max_particles_amount = 0;
        float emission_frequency = 0.0f;
        float min_particles_lifetime = 0.0f;
        float max_particles_lifetime = 0.0f;
        bool autostart_emission = true;

    public:
        INIT_BUILDER_STUB(Draw_Module__Particle)

    };

}
