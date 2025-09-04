#pragma once

#include <Stuff/Timer.h>

#include <Module.h>


namespace LMD
{

    class Timer_Module : public LEti::Module
    {
    public:
        INIT_VARIABLE(LMD::Timer_Module, LEti::Module)

    private:
        LST::Timer m_timer;

    public:
        LST::Timer& timer() { return m_timer; }
        const LST::Timer& timer() const { return m_timer; }

    public:
        void update(float _dt) override;

    };


    class Timer_Module_Stub : public LEti::Module_Stub
    {
    public:
        INIT_VARIABLE(LMD::Timer_Module_Stub, LEti::Module_Stub)

    public:
        INIT_BUILDER_STUB(Timer_Module)

    };

}
