#include <Misc_Draw_Modules/Vertex_Animation/Shader_Component__Vertex_Animation.h>

#include <glew.h>

#include <Misc_Draw_Modules/Vertex_Animation/Draw_Module__Vertex_Animation.h>

using namespace LMD;


void Shader_Component__Vertex_Animation::init(unsigned int _opengl_program_handle)
{
    Parent_Type::init(_opengl_program_handle);

    m_frames_amount_uniform = M_extract_uniform_location(m_frames_amount_uniform_name);
    m_current_frame_uniform = M_extract_uniform_location(m_current_frame_uniform_name);
    m_vertices_per_frame_uniform = M_extract_uniform_location(m_vertices_per_frame_uniform_name);
    m_mix_ratio_uniform = M_extract_uniform_location(m_mix_ratio_uniform_name);
}

void Shader_Component__Vertex_Animation::update(const LR::Draw_Module* _draw_module)
{
    Parent_Type::update(_draw_module);

    L_ASSERT(LV::cast_variable<Draw_Module__Vertex_Animation>(_draw_module));
    Draw_Module__Vertex_Animation* draw_module = (Draw_Module__Vertex_Animation*)_draw_module;

    glUniform1ui(m_frames_amount_uniform, draw_module->frames_amount());
    glUniform1ui(m_current_frame_uniform, draw_module->current_frame());
    glUniform1ui(m_vertices_per_frame_uniform, draw_module->vertices_per_frame());
    glUniform1f(m_mix_ratio_uniform, draw_module->frame_ratio());
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Shader_Component_Stub__Vertex_Animation)

BUILDER_STUB_INITIALIZATION_FUNC(Shader_Component_Stub__Vertex_Animation)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_frames_amount_uniform_name(frames_amount_uniform_name);
    product->set_current_frame_uniform_name(current_frame_uniform_name);
    product->set_vertices_per_frame_uniform_name(vertices_per_frame_uniform_name);
    product->set_mix_ratio_uniform_name(mix_ratio_uniform_name);
}
