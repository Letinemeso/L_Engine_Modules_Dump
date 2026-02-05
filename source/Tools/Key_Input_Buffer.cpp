#include <Tools/Key_Input_Buffer.h>

#include <Window/Window_Controller.h>

using namespace LMD;


Key_Input_Buffer::Key_Input_Buffer()
{
    m_keys_timers.resize_and_fill(LR::Keys_Amount, {});
    m_mouse_buttons_timers.resize_and_fill(LR::Mouse_Buttons_Amount, {});
}

Key_Input_Buffer::~Key_Input_Buffer()
{

}



bool Key_Input_Buffer::key_pressed(unsigned int _id) const
{
    L_ASSERT(_id < LR::Keys_Amount);

    return m_keys_timers[_id].is_active();
}

bool Key_Input_Buffer::check_key_pressed_and_reset(unsigned int _id)
{
    bool result = key_pressed(_id);
    reset_key_activeness(_id);
    return result;
}


bool Key_Input_Buffer::mouse_button_pressed(unsigned int _id) const
{
    L_ASSERT(_id < LR::Mouse_Buttons_Amount);

    return m_mouse_buttons_timers[_id].is_active();
}

bool Key_Input_Buffer::check_mouse_button_pressed_and_reset(unsigned int _id)
{
    bool result = mouse_button_pressed(_id);
    reset_mouse_button_activeness(_id);
    return result;
}


void Key_Input_Buffer::reset_key_activeness(unsigned int _id)
{
    L_ASSERT(_id < LR::Keys_Amount);
    m_keys_timers[_id].reset();
}

void Key_Input_Buffer::reset_mouse_button_activeness(unsigned int _id)
{
    L_ASSERT(_id < LR::Mouse_Buttons_Amount);
    m_mouse_buttons_timers[_id].reset();
}

void Key_Input_Buffer::reset()
{
    for(unsigned int i = 0; i < LR::Keys_Amount; ++i)
        m_keys_timers[i].reset();
    for(unsigned int i = 0; i < LR::Mouse_Buttons_Amount; ++i)
        m_mouse_buttons_timers[i].reset();
}



void Key_Input_Buffer::update(float _dt)
{
    for(unsigned int i = 0; i < LR::Keys_Amount; ++i)
    {
        m_keys_timers[i].update(_dt);

        if(LR::Window_Controller::instance().key_was_pressed(i))
            m_keys_timers[i].start(m_activeness_duration);
    }

    for(unsigned int i = 0; i < LR::Mouse_Buttons_Amount; ++i)
    {
        m_mouse_buttons_timers[i].update(_dt);

        if(LR::Window_Controller::instance().mouse_button_was_pressed(i))
            m_mouse_buttons_timers[i].start(m_activeness_duration);
    }
}
