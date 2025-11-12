#include <Misc_Draw_Modules/Vertex_Animation/Draw_Module__Vertex_Animation.h>

#include <glew.h>
#include <Shader/Shader_Program.h>

#include <Resources/Vertex_Animation.h>

using namespace LMD;


void Draw_Module__Vertex_Animation::start(unsigned int _cycles, unsigned int _with_frame)
{
    m_times_to_repeat = _cycles;
    m_repetitions = 0;

    set_frame(_with_frame);

    unpause();
}

void Draw_Module__Vertex_Animation::pause()
{
    m_is_paused = true;
}

void Draw_Module__Vertex_Animation::unpause()
{
    m_is_paused = false;
}

void Draw_Module__Vertex_Animation::set_cycles(unsigned int _cycles)
{
    m_repetitions = _cycles;
    m_repetitions = 0;
}

void Draw_Module__Vertex_Animation::set_frame(unsigned int _frame)
{
    m_current_frame = _frame;
    m_time_before_next_frame = 0.0f;
    m_frame_ratio = 0.0f;
}



void Draw_Module__Vertex_Animation::M_update_internal(float _dt)
{
    Parent_Type::M_update_internal(_dt);

    if(m_is_paused)
        return;

    if(m_current_frame + 1 >= frames_amount())
        ++m_repetitions;

    if(m_repetitions >= m_times_to_repeat && m_times_to_repeat != 0)
    {
        pause();
        return;
    }

    m_time_before_next_frame += _dt;
    m_frame_ratio = m_time_before_next_frame / m_time_per_frame;

    if(m_time_before_next_frame > m_time_per_frame)
        set_frame((m_current_frame + 1) % frames_amount());
}

void Draw_Module__Vertex_Animation::M_draw_internal() const
{
    glDrawArrays(draw_mode(), 0, m_vertices_per_frame);
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Draw_Module_Stub__Vertex_Animation)

BUILDER_STUB_INITIALIZATION_FUNC(Draw_Module_Stub__Vertex_Animation)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    L_ASSERT(resources_manager);
    L_ASSERT(animation_name.size() > 0);
    L_ASSERT(frames_per_second > 0.0f);

    const Vertex_Animation* animation = resources_manager->get_resource<Vertex_Animation>(animation_name);

    product->set_frames_amount(animation->frames_amount());
    product->set_vertices_per_frame(animation->vertices_per_frame());
    product->set_fps(frames_per_second);

    product->start(times_to_repeat);
}
