#include <Misc_Draw_Modules/Animation/Graphics_Component__Animation/Graphics_Component__Animation.h>

#include <Resources/Animation.h>

using namespace LMD;


Graphics_Component__Animation::Graphics_Component__Animation()
{

}

Graphics_Component__Animation::~Graphics_Component__Animation()
{
    for(unsigned int i = 0; i < m_frame_textures.size(); ++i)
        delete m_frame_textures[i];
}



void Graphics_Component__Animation::set_frame_textures(Frame_Textures&& _frame_textures)
{
    for(unsigned int i = 0; i < m_frame_textures.size(); ++i)
        delete m_frame_textures[i];

    m_frame_textures = (Frame_Textures&&)_frame_textures;

    m_frame_offset_ratio = 1.0f / (float)frames_amount();
}


void Graphics_Component__Animation::set_fps(float _fps)
{
    m_fps = _fps;
    m_time_before_next_frame = 1.0f / m_fps;
}

void Graphics_Component__Animation::set_frame(unsigned int _frame)
{
    m_current_frame = _frame;
    m_frame_update_timer.start(m_time_before_next_frame);
}


void Graphics_Component__Animation::start(unsigned int _cycles, unsigned int _with_frame)
{
    pause();

    m_times_to_repeat = _cycles;
    m_repetitions = 0;

    set_frame(_with_frame);
    unpause();
}

void Graphics_Component__Animation::pause()
{
    m_frame_update_timer.reset();
    m_is_paused = true;
}

void Graphics_Component__Animation::unpause()
{
    if(!m_frame_update_timer.is_active())
        m_frame_update_timer.start(m_time_before_next_frame);
    m_is_paused = false;
}

void Graphics_Component__Animation::set_cycles(unsigned int _cycles)
{
    m_repetitions = _cycles;
    m_repetitions = 0;
}



void Graphics_Component__Animation::reconfigure_texture_coords()
{
    L_ASSERT(frames_amount());
    L_ASSERT(m_buffer.size() > 0);
    L_ASSERT(m_buffer.floats_per_vertex() >= 2);

    const LR::Texture* first_frame_texture = m_frame_textures[0];

    LR::Buffer::Element_Modification_Func modification_func = [first_frame_texture](float& _element, unsigned int _index)
    {
        float* elements_as_array = &_element;
        elements_as_array[0] /= first_frame_texture->width();
        elements_as_array[1] /= first_frame_texture->height();
    };

    m_buffer.modify_buffer(modification_func, 0, LR::Buffer::All_Elements, m_buffer.floats_per_vertex());
}



void Graphics_Component__Animation::update(float _dt)
{
    if(m_is_paused)
        return;

    m_frame_update_timer.update(_dt);
    if(m_frame_update_timer.is_active())
        return;

    if(m_current_frame + 1 >= frames_amount())
        ++m_repetitions;

    if(m_repetitions >= m_times_to_repeat && m_times_to_repeat != 0)
    {
        pause();
        return;
    }

    set_frame((m_current_frame + 1) % frames_amount());
}

void Graphics_Component__Animation::prepare_to_draw() const
{
    Parent_Type::prepare_to_draw();

    L_ASSERT(frames_amount() > m_current_frame);

    m_frame_textures[m_current_frame]->prepare_to_draw();
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Graphics_Component_Stub__Animation)

BUILDER_STUB_INITIALIZATION_FUNC(Graphics_Component_Stub__Animation)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    const Animation* animation = resources_manager->get_resource<Animation>(animation_name);

    Graphics_Component__Animation::Frame_Textures textures(animation->frames_amount(), nullptr);
    for(unsigned int i = 0; i < animation->frames_amount(); ++i)
    {
        LR::Texture__Preloaded* texture = new LR::Texture__Preloaded;
        textures[i] = texture;
        texture_settings.apply_settings(texture);
        texture->set_picture(animation->get_frame(i));
    }

    product->set_frame_textures((Graphics_Component__Animation::Frame_Textures&&)textures);
    product->set_fps(frames_per_second);

    if(texture_coords_in_pixels)
        product->reconfigure_texture_coords();

    product->start(times_to_repeat);
}
