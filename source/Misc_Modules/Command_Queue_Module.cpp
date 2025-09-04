#include <Misc_Modules/Command_Queue_Module.h>

using namespace LMD;


void Command_Queue_Module::update(float _dt)
{
    m_command_queue.process();
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Command_Queue_Module_Stub)

BUILDER_STUB_INITIALIZATION_FUNC(Command_Queue_Module_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
}
