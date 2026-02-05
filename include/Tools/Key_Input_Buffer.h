#pragma once

#include <Data_Structures/Vector.h>
#include <Stuff/Timer.h>


namespace LMD
{

    class Key_Input_Buffer
    {
    private:
        LDS::Vector<LST::Timer> m_keys_timers;
        LDS::Vector<LST::Timer> m_mouse_buttons_timers;

        float m_activeness_duration = 0.1f;

    public:
        Key_Input_Buffer();
        ~Key_Input_Buffer();

    public:
        inline void set_activeness_duration(float _value) { m_activeness_duration = _value; }

    public:
        bool key_pressed(unsigned int _id) const;
        bool check_key_pressed_and_reset(unsigned int _id);

        bool mouse_button_pressed(unsigned int _id) const;
        bool check_mouse_button_pressed_and_reset(unsigned int _id);

        void reset_key_activeness(unsigned int _id);
        void reset_mouse_button_activeness(unsigned int _id);
        void reset();

    public:
        void update(float _dt);

    };

}
