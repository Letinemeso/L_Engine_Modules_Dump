#pragma once

#include <Stuff/Timer.h>
#include <Stuff/Function_Wrapper.h>

#include <Module.h>


namespace LMD
{

    class Expiration_Module : public LEti::Module
    {
    public:
        INIT_VARIABLE(LMD::Expiration_Module, LEti::Module)

    private:
        LST::Timer m_timer;

        LST::Function<void(LEti::Object*)> m_on_expired_function;

    public:
        inline void start_timer(float _time) { m_timer.start(_time); }
        inline void set_on_expired_function(const LST::Function<void(LEti::Object*)>& _func) { m_on_expired_function = _func; }

    public:
        void update(float _dt) override;

    };


    class Expiration_Module_Stub : public LEti::Module_Stub         //    this one is not registered with the rest of Dump's content due
    {                                                               //    to undefined behavior in case of expiration (should be configured by application)
    public:
        INIT_VARIABLE(LMD::Expiration_Module_Stub, LEti::Module_Stub)

        INIT_FIELDS
        ADD_FIELD(float, expiration_time)
        FIELDS_END

    public:
        float expiration_time = 0.0f;

    public:
        LST::Function<void(LEti::Object*)> on_expired_function;

    public:
        INIT_BUILDER_STUB(Expiration_Module)

    };

}
