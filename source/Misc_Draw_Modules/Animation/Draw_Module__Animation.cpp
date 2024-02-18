#include <Misc_Draw_Modules/Animation/Draw_Module__Animation.h>

using namespace LMD;


void Draw_Module__Animation::M_update_frame(float _dt)
{
    if(m_is_paused)
        return;

    m_frame_update_timer.update(_dt);
    if(m_frame_update_timer.is_active())
        return;

    if(m_current_frame + 1 >= m_frames_count)
        ++m_repetitions;

    if(m_repetitions >= m_times_to_repeat && m_times_to_repeat != 0)
    {
        pause();
        return;
    }

    set_frame((m_current_frame + 1) % m_frames_count);
    m_frame_update_timer.start(m_time_before_next_frame);
}



void Draw_Module__Animation::update(float _dt)
{
    M_update_frame(_dt);

    Draw_Module::update(_dt);
}



void Draw_Module__Animation::set_animation_data(unsigned int _frames_count)
{
    m_frames_count = _frames_count;
    m_frame_offset_ratio = 1.0f / (float)m_frames_count;
}

void Draw_Module__Animation::set_fps(float _fps)
{
    m_fps = _fps;
    m_time_before_next_frame = 1.0f / m_fps;
}

void Draw_Module__Animation::set_frame(unsigned int _frame)
{
    float current_offset = m_current_frame * m_frame_offset_ratio;
    float needed_offset = _frame * m_frame_offset_ratio;

    for(unsigned int i=0; i<m_texture->buffer().size(); i += 2)
        m_texture->buffer()[i] += needed_offset - current_offset;

    m_current_frame = _frame;
}


void Draw_Module__Animation::start(unsigned int _cycles)
{
    pause();

    m_times_to_repeat = _cycles;
    m_repetitions = 0;

    set_frame(0);
    unpause();
}

void Draw_Module__Animation::pause()
{
    m_frame_update_timer.reset();
    m_is_paused = true;
}

void Draw_Module__Animation::unpause()
{
    if(!m_frame_update_timer.is_active())
        m_frame_update_timer.start(m_time_before_next_frame);
    m_is_paused = false;
}






Draw_Module__Animation__Stub::~Draw_Module__Animation__Stub()
{
    delete texture_stub;
}



LV::Variable_Base* Draw_Module__Animation__Stub::M_construct_product() const
{
    return new Draw_Module__Animation;
}

void Draw_Module__Animation__Stub::M_init_constructed_product(LV::Variable_Base *_product) const
{
    Draw_Module_Stub::M_init_constructed_product(_product);

    Draw_Module__Animation* product = (Draw_Module__Animation*)_product;

    LR::Graphics_Component__Texture* texture = (LR::Graphics_Component__Texture*)texture_stub->construct();

    product->set_texture(texture);
    product->add_graphics_component(texture);

    product->set_animation_data(frames_count);
    product->set_fps(frames_per_second);

    product->start(times_to_repeat);
}

