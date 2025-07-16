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
        LEti::Transformation_Data* m_parent_transformation_data = nullptr;
        const LEti::Transformation_Data* m_parent_transformation_data_prev_state = nullptr;

        LEti::Transformation_Data m_initial_transformation;

    private:
        LR::Shader_Program* m_initialization_shader = nullptr;
        LR::Shader_Program* m_update_shader = nullptr;

        LR::Draw_Module::Uniform_Setter_List m_initialization_uniform_setters;

        unsigned int m_max_particles = 0;
        float m_emission_frequency = 0.0f;
        bool m_emission_is_paused = false;

        LST::Timer m_emission_timer;
        unsigned int m_requested_particles_amount = 0;
        unsigned int m_alive_particles_amount = 0;

    public:
        Draw_Module__Particle();
        ~Draw_Module__Particle();

    public:
        inline void set_initialization_shader(LR::Shader_Program* _ptr) { m_initialization_shader = _ptr; M_init_uniform_setters(m_initialization_uniform_setters, m_initialization_shader); }

        inline void set_max_particles(unsigned int _value) { m_max_particles = _value; }
        inline void set_emission_frequency(float _value) { m_emission_frequency = _value; }
        inline void pause_emission(bool _value) { m_emission_is_paused = _value; }
        inline void reset_requested_particles_amount() { m_requested_particles_amount = 0; }
        inline void update_alive_particles_amount(unsigned int _value) { m_alive_particles_amount = _value; }

        inline unsigned int max_particles() const { return m_max_particles; }
        inline float emission_frequency() const { return m_emission_frequency; }
        inline unsigned int alive_particles_amount() const { return m_alive_particles_amount; }
        inline unsigned int requested_particles_amount() const { return m_requested_particles_amount; }
        inline bool emission_is_paused() const { return m_emission_is_paused; }

        inline const LEti::Transformation_Data* parent_transformation_data() const { return m_parent_transformation_data; }
        inline const LEti::Transformation_Data* parent_transformation_data_prev_state() const { return m_parent_transformation_data; }

    public:
        void add_initialization_compute_uniform_setter(LR::Uniform_Setter* _ptr);

    public:
        LR::Uniform_Setter* get_initialization_compute_uniform_setter_with_name(const std::string& _name) const;

    public:
        void set_transformation_data(LEti::Transformation_Data* _data) override;
        void set_transformation_data_prev_state(const LEti::Transformation_Data* _data) override;

    public:
        void request_particles(unsigned int _amount);

    private:
        void M_update_emission_timer(float _dt);

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
        ADD_FIELD(bool, autostart_emission)
        ADD_FIELD(std::string, particle_initialization_compute_shader)
        FIELDS_END

        INIT_CHILDS_LISTS
        ADD_CHILDS_LIST("Initialization_Compute_Uniform_Setter_Stub__*", &initialization_compute_uniform_setter_stubs)
        CHILDS_LISTS_END

    public:
        unsigned int max_particles_amount = 0;
        float emission_frequency = 0.0f;
        bool autostart_emission = true;

        std::string particle_initialization_compute_shader;

    public:
        LV::Variable_Base::Childs_List initialization_compute_uniform_setter_stubs;

    public:
        INIT_BUILDER_STUB(Draw_Module__Particle)

    public:
        ~Draw_Module_Stub__Particle();

    };

}
