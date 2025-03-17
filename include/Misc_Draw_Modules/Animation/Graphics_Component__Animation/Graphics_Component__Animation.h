#pragma once

#include <Stuff/Timer.h>

#include <Resources_Manager.h>

#include <Components/Graphics_Component.h>
#include <Texture/Texture__Preloaded.h>


namespace LMD
{

    class Graphics_Component__Animation : public LR::Graphics_Component
    {
    public:
        INIT_VARIABLE(LMD::Graphics_Component__Animation, LR::Graphics_Component)

    public:
        using Frame_Textures = LDS::Vector<LR::Texture__Preloaded*>;

    private:
        Frame_Textures m_frame_textures;

    private:
        float m_fps = 0;
        float m_time_before_next_frame = 0.0f;
        float m_frame_offset_ratio = 0.0f;

        unsigned int m_current_frame = 0;
        bool m_is_paused = true;
        unsigned int m_times_to_repeat = 0;
        unsigned int m_repetitions = 0;
        LST::Timer m_frame_update_timer;

    public:
        Graphics_Component__Animation();
        ~Graphics_Component__Animation();

    public:
        inline unsigned int frames_amount() const { return m_frame_textures.size(); }
        inline float fps() const { return m_fps; }
        inline unsigned int current_frame() const { return m_current_frame; }
        inline unsigned int repetitions() const { return m_repetitions; }
        inline bool paused() const { return m_is_paused; }

    public:
        void set_frame_textures(Frame_Textures&& _frame_textures);

        void set_fps(float _fps);
        void set_frame(unsigned int _frame);

        void start(unsigned int _cycles = 0, unsigned int _with_frame = 0);
        void pause();
        void unpause();
        void set_cycles(unsigned int _cycles);

    public:
        void reconfigure_texture_coords();

    public:
        void update(float _dt) override;
        void prepare_to_draw() const override;

    };


    class Graphics_Component_Stub__Animation : public LR::Graphics_Component_Stub
    {
    public:
        INIT_VARIABLE(LMD::Graphics_Component_Stub__Animation, LR::Graphics_Component_Stub)

        INIT_FIELDS
        ADD_FIELD(std::string, animation_name)
        ADD_FIELD(float, frames_per_second)
        ADD_FIELD(unsigned int, times_to_repeat)
        ADD_FIELD(bool, texture_coords_in_pixels)
        FIELDS_END

        INIT_CHILDS
        ADD_CHILD("texture_settings", texture_stub_ptr)
        CHILDS_END

    public:
        const LEti::Resources_Manager* resources_manager = nullptr;

    public:
        std::string animation_name;
        float frames_per_second = 0;
        unsigned int times_to_repeat = 0;
        bool texture_coords_in_pixels = true;

        LR::Texture_Stub texture_settings;

    private:
        LR::Texture_Stub* texture_stub_ptr = &texture_settings;

    public:
        INIT_BUILDER_STUB(Graphics_Component__Animation)

    };

}
