#pragma once

#include <Resources_Manager.h>

#include <Draw_Modules/Draw_Module.h>


namespace LMD
{

    class Draw_Module__Vertex_Animation : public LR::Draw_Module
    {
    public:
        INIT_VARIABLE(LMD::Draw_Module__Vertex_Animation, LR::Draw_Module)

    private:
        unsigned int m_frames_amount = 0;
        unsigned int m_vertices_per_frame = 0;
        float m_fps = 0.0f;
        float m_time_per_frame = 0.0f;
        unsigned int m_times_to_repeat = 0;

    private:
        bool m_is_paused = true;
        unsigned int m_current_frame = 0;
        float m_time_before_next_frame = 0.0f;
        float m_frame_ratio = 0.0f;
        unsigned int m_repetitions = 0;

    public:
        inline void set_frames_amount(unsigned int _value) { m_frames_amount = _value; m_time_per_frame = (float)m_frames_amount / m_fps; }
        inline void set_vertices_per_frame(unsigned int _value) { m_vertices_per_frame = _value; }
        inline void set_fps(float _value) { m_fps = _value; m_time_per_frame = (float)m_frames_amount / m_fps; }
        inline void set_times_to_repeat(unsigned int _value) { m_times_to_repeat = _value; }

    public:
        void start(unsigned int _cycles = 0, unsigned int _with_frame = 0);
        void pause();
        void unpause();
        void set_cycles(unsigned int _cycles);
        void set_frame(unsigned int _frame);

    public:
        inline unsigned int frames_amount() const { return m_frames_amount; }
        inline unsigned int vertices_per_frame() const { return m_vertices_per_frame; }
        inline float fps() const { return m_fps; }
        inline unsigned int current_frame() const { return m_current_frame; }
        inline unsigned int repetitions() const { return m_repetitions; }
        inline bool paused() const { return m_is_paused; }
        inline float frame_ratio() const { return m_frame_ratio; }

    protected:
        void M_update_internal(float _dt) override;
        void M_draw_internal() const override;

    };


    class Draw_Module_Stub__Vertex_Animation : public LR::Draw_Module_Stub
    {
    public:
        INIT_VARIABLE(LMD::Draw_Module_Stub__Vertex_Animation, LR::Draw_Module_Stub)

        INIT_FIELDS
        ADD_FIELD(std::string, animation_name)
        ADD_FIELD(float, frames_per_second)
        ADD_FIELD(unsigned int, times_to_repeat)
        FIELDS_END

    public:
        const LEti::Resources_Manager* resources_manager = nullptr;

    public:
        std::string animation_name;
        float frames_per_second = 0.0f;
        unsigned int times_to_repeat = 0;

    public:
        INIT_BUILDER_STUB(Draw_Module__Vertex_Animation)

    };

}
