#include <Misc_Modules/Expiration_Module.h>

using namespace LMD;


void Expiration_Module::update(float _dt)
{
    L_ASSERT(m_on_expired_function);

    m_timer.update(_dt);

    if(!m_timer.is_active())
        m_on_expired_function(parent_object());
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Expiration_Module_Stub)

BUILDER_STUB_INITIALIZATION_FUNC(Expiration_Module_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    L_ASSERT(on_expired_function);

    product->set_on_expired_function(on_expired_function);
    product->start_timer(expiration_time);
}
