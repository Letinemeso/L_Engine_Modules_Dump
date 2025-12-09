#pragma once

#include <Shader/Shader_Components/Shader_Component.h>


namespace LMD
{

    class Shader_Component__Vertex_Animation : public LR::Shader_Component
    {
    public:
        INIT_VARIABLE(LMD::Shader_Component__Vertex_Animation, LR::Shader_Component)

    private:
        std::string m_frames_amount_uniform_name;
        int m_frames_amount_uniform = -1;
        std::string m_current_frame_uniform_name;
        int m_current_frame_uniform = -1;
        std::string m_vertices_per_frame_uniform_name;
        int m_vertices_per_frame_uniform = -1;
        std::string m_mix_ratio_uniform_name;
        int m_mix_ratio_uniform = -1;

    public:
        inline void set_frames_amount_uniform_name(const std::string& _value) { m_frames_amount_uniform_name = _value; }
        inline void set_current_frame_uniform_name(const std::string& _value) { m_current_frame_uniform_name = _value; }
        inline void set_vertices_per_frame_uniform_name(const std::string& _value) { m_vertices_per_frame_uniform_name = _value; }
        inline void set_mix_ratio_uniform_name(const std::string& _value) { m_mix_ratio_uniform_name = _value; }

    public:
        void init(unsigned int _opengl_program_handle) override;

    public:
        void update(const LR::Draw_Module* _draw_module) override;

    };


    class Shader_Component_Stub__Vertex_Animation : public LR::Predefined_Shader_Component_Stub
    {
    public:
        INIT_VARIABLE(LMD::Shader_Component_Stub__Vertex_Animation, LR::Predefined_Shader_Component_Stub)

        INIT_FIELDS
        ADD_FIELD(std::string, frames_amount_uniform_name)
        ADD_FIELD(std::string, current_frame_uniform_name)
        ADD_FIELD(std::string, vertices_per_frame_uniform_name)
        ADD_FIELD(std::string, mix_ratio_uniform_name)
        FIELDS_END

    public:
        std::string frames_amount_uniform_name = "frames_amount";
        std::string current_frame_uniform_name = "current_frame";
        std::string vertices_per_frame_uniform_name = "vertices_per_frame";
        std::string mix_ratio_uniform_name = "mix_ratio";

    public:
        INIT_BUILDER_STUB(Shader_Component__Vertex_Animation)

    };

}
