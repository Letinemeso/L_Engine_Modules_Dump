#include <Misc_Modules/Custom_Module.h>

using namespace LMD;


void Custom_Module::update(float _dt)
{
    L_ASSERT(m_update_function);

    m_update_function(parent_object(), _dt);
}
