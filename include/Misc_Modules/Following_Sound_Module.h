#pragma once

#include <Module.h>

#include <Sound.h>


namespace LMD
{

    class Following_Sound_Module : public LEti::Module
    {
    public:
        INIT_VARIABLE(LMD::Following_Sound_Module, LEti::Module)

    private:
        LSound::Sound* m_sound = nullptr;
        bool m_started_sound = false;

    public:
        Following_Sound_Module();
        ~Following_Sound_Module();

    public:
        inline void set_sound(LSound::Sound* _ptr) { delete m_sound; m_sound = _ptr; }
        inline LSound::Sound* sound() { return m_sound; }

    public:
        void update(float _dt) override;

    };


    class Following_Sound_Module_Stub : public LEti::Module_Stub
    {
    public:
        INIT_VARIABLE(LMD::Following_Sound_Module_Stub, LEti::Module_Stub)

        INIT_CHILDS
        ADD_CHILD("sound_stub", sound_stub)
        CHILDS_END

    public:
        LSound::Sound_Stub* sound_stub = nullptr;

    public:
        INIT_BUILDER_STUB(Following_Sound_Module)

    public:
        ~Following_Sound_Module_Stub();

    };

}
