#pragma once

#include <vec2.hpp>
#include <vec3.hpp>

#include <Stuff/Timer.h>

#include <Components/Graphics_Component_Reconstructor.h>
#include <Components/Graphics_Component__Texture.h>


namespace LMD
{

    class Animation_Graphics_Component_Reconstructor__Texture : public LR::Graphics_Component_Reconstructor
    {
    public:
        INIT_VARIABLE(LMD::Animation_Graphics_Component_Reconstructor__Texture, LR::Graphics_Component_Reconstructor)

   private:
       unsigned int m_frames_count = 0;
       float m_fps = 0;
       float m_time_before_next_frame = 0.0f;
       float m_frame_offset_ratio = 0.0f;

       unsigned int m_current_frame = 0;
       unsigned int m_requested_frame = 0;

   private:
       bool m_is_paused = true;
       LST::Timer m_frame_update_timer;
       unsigned int m_times_to_repeat = 0;
       unsigned int m_repetitions = 0;

    public:
        void set_animation_data(unsigned int _frames_count);
        void set_fps(float _fps);
        void set_frame(unsigned int _frame);

        void start(unsigned int _cycles = 0, unsigned int _with_frame = 0);
        void pause();
        void unpause();
        void set_cycles(unsigned int _cycles);

    public:
        inline unsigned int frames_count() const { return m_frames_count; }
        inline float fps() const { return m_fps; }
        inline unsigned int current_frame() const { return m_current_frame; }
        inline unsigned int repetitions() const { return m_repetitions; }
        inline bool paused() const { return m_is_paused; }

    private:
       void M_recalculate_frame_data(LR::Graphics_Component__Texture& _texture);

    public:
        void update(float _dt) override;

    };


    class Animation_Graphics_Component_Reconstructor_Stub__Texture : public LR::Graphics_Component_Reconstructor_Stub
    {
    public:
        INIT_VARIABLE(LMD::Animation_Graphics_Component_Reconstructor_Stub__Texture, LR::Graphics_Component_Reconstructor_Stub)

        INIT_FIELDS
        ADD_FIELD(float, frames_per_second)
        ADD_FIELD(unsigned int, frames_count)
        ADD_FIELD(unsigned int, times_to_repeat)
        FIELDS_END

    public:
        float frames_per_second = 0;
        unsigned int frames_count = 0;
        unsigned int times_to_repeat = 0;

    protected:
        INIT_BUILDER_STUB(Animation_Graphics_Component_Reconstructor__Texture)

    };

}
