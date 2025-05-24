#pragma once

#include <Stuff/Timer.h>

#include <Components/Graphics_Component.h>

#include <Misc_Draw_Modules/Particle_Draw_Module/Draw_Module__Particle.h>


namespace LMD
{

    class Graphics_Component__Particles_Amount_Reader : public LR::Graphics_Component
    {
    public:
        INIT_VARIABLE(LMD::Graphics_Component__Particles_Amount_Reader, LR::Graphics_Component)

    public:
        unsigned int m_binding_point_index = 0xFFFFFFFF;
        float m_update_frequency = 0.0f;

        LST::Timer m_timer;

        unsigned int m_counter_buffer = 0;
        unsigned int m_read_buffer = 0;

    public:
        Graphics_Component__Particles_Amount_Reader();
        ~Graphics_Component__Particles_Amount_Reader();

    public:
        inline void set_binding_point_index(unsigned int _value) { m_binding_point_index = _value; }
        inline void set_update_frequency(float _value) { m_update_frequency = _value; m_timer.start(m_update_frequency); }

    private:
        Draw_Module__Particle* draw_module() const { L_ASSERT(LV::cast_variable<Draw_Module__Particle>(m_parent_draw_module)); return (Draw_Module__Particle*)m_parent_draw_module; }

    private:
        void M_extract_data() const;
        void M_fill_requested_particles() const;

    public:
        unsigned int layout_index() const override;
        unsigned int vertices_amount() const override;
        unsigned int required_compute_shader_invocations() const override;
        void setup_buffer() const override;
        void update(float _dt) override;
        void prepare_to_draw() const override;
        void bind_for_computation() const override;

    };


    class Graphics_Component_Stub__Particles_Amount_Reader : public LR::Graphics_Component_Stub
    {
    public:
        INIT_VARIABLE(LMD::Graphics_Component_Stub__Particles_Amount_Reader, LR::Graphics_Component_Stub)

        INIT_FIELDS
        ADD_FIELD(unsigned int, binding_point_index)
        ADD_FIELD(float, update_frequency)
        FIELDS_END

    public:
        unsigned int binding_point_index = 0xFFFFFFFF;
        float update_frequency = 0.0f;

    public:
        INIT_BUILDER_STUB(Graphics_Component__Particles_Amount_Reader)

    };

}
