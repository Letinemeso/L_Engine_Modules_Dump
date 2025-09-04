#pragma once

#include <Stuff/Command_Queue.h>

#include <Module.h>


namespace LMD
{

    class Command_Queue_Module : public LEti::Module
    {
    public:
        INIT_VARIABLE(LMD::Command_Queue_Module, LEti::Module);

    private:
        LST::Command_Queue m_command_queue;

    public:
        inline LST::Command_Queue& queue() { return m_command_queue; }
        inline const LST::Command_Queue& queue() const { return m_command_queue; }

    public:
        void update(float _dt) override;

    };


    class Command_Queue_Module_Stub : public LEti::Module_Stub
    {
    public:
        INIT_VARIABLE(LMD::Command_Queue_Module_Stub, LEti::Module_Stub);

    public:
        INIT_BUILDER_STUB(Command_Queue_Module);

    };

}
