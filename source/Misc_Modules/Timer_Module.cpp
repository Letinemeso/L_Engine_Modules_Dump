#include <Misc_Modules/Timer_Module.h>

using namespace LMD;


void Timer_Module::update(float _dt)
{
    m_timer.update(_dt);
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Timer_Module_Stub)

BUILDER_STUB_INITIALIZATION_FUNC(Timer_Module_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
}
