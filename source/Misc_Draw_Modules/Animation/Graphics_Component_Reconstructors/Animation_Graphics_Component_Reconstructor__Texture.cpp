#include <Misc_Draw_Modules/Animation/Graphics_Component_Reconstructors/Animation_Graphics_Component_Reconstructor__Texture.h>

#include <Draw_Modules/Draw_Module.h>

using namespace LMD;


void Animation_Graphics_Component_Reconstructor__Texture::set_animation_data(unsigned int _frames_count)
{
    m_frames_count = _frames_count;
    m_frame_offset_ratio = 1.0f / (float)m_frames_count;
}

void Animation_Graphics_Component_Reconstructor__Texture::set_fps(float _fps)
{
    m_fps = _fps;
    m_time_before_next_frame = 1.0f / m_fps;
}

void Animation_Graphics_Component_Reconstructor__Texture::set_frame(unsigned int _frame)
{
    m_requested_frame = _frame;
    m_frame_update_timer.start(m_time_before_next_frame);
}


void Animation_Graphics_Component_Reconstructor__Texture::start(unsigned int _cycles, unsigned int _with_frame)
{
    pause();

    m_times_to_repeat = _cycles;
    m_repetitions = 0;

    set_frame(_with_frame);
    unpause();

    if(m_graphics_component)
        update(0.0f);
}

void Animation_Graphics_Component_Reconstructor__Texture::pause()
{
    m_frame_update_timer.reset();
    m_is_paused = true;
}

void Animation_Graphics_Component_Reconstructor__Texture::unpause()
{
    if(!m_frame_update_timer.is_active())
        m_frame_update_timer.start(m_time_before_next_frame);
    m_is_paused = false;
}

void Animation_Graphics_Component_Reconstructor__Texture::set_cycles(unsigned int _cycles)
{
    m_repetitions = _cycles;
    m_repetitions = 0;
}



void Animation_Graphics_Component_Reconstructor__Texture::M_recalculate_frame_data(LR::Graphics_Component__Texture& _texture)
{
    if(m_current_frame == m_requested_frame)
        return;

    float current_offset = m_current_frame * m_frame_offset_ratio;
    float needed_offset = m_requested_frame * m_frame_offset_ratio;
    float modifier = needed_offset - current_offset;

    LR::Buffer::Element_Modification_Func modification_func = [modifier](float& _element, unsigned int _index)
    {
        _element += modifier;
    };

    m_draw_module->bind_vertex_array();
    _texture.buffer().modify_buffer(modification_func, 0, _texture.buffer().size(), _texture.buffer().floats_per_vertex());

    m_current_frame = m_requested_frame;
}



void Animation_Graphics_Component_Reconstructor__Texture::update(float _dt)
{
    L_ASSERT(LV::cast_variable<LR::Graphics_Component__Texture>(m_graphics_component));

    M_recalculate_frame_data(*(LR::Graphics_Component__Texture*)m_graphics_component);

    if(m_is_paused)
        return;

    m_frame_update_timer.update(_dt);
    if(m_frame_update_timer.is_active())
        return;

    if(m_requested_frame + 1 >= m_frames_count)
        ++m_repetitions;

    if(m_repetitions >= m_times_to_repeat && m_times_to_repeat != 0)
    {
        pause();
        return;
    }

    set_frame((m_requested_frame + 1) % m_frames_count);
    m_frame_update_timer.start(m_time_before_next_frame);
}





BUILDER_STUB_CONSTRUCTION_FUNC(Animation_Graphics_Component_Reconstructor_Stub__Texture) BUILDER_STUB_CONSTRUCTION_FUNC_DEFAULT_IMPL

BUILDER_STUB_INITIALIZATION_FUNC(Animation_Graphics_Component_Reconstructor_Stub__Texture)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_animation_data(frames_count);
    product->set_fps(frames_per_second);

    product->start(times_to_repeat);
}
