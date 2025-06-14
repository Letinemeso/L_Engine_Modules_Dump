#include <Misc_Modules/Following_Sound_Module.h>

using namespace LMD;


Following_Sound_Module::Following_Sound_Module()
{

}

Following_Sound_Module::~Following_Sound_Module()
{
    delete m_sound;
}



void Following_Sound_Module::update(float _dt)
{
    L_ASSERT(m_sound);

    if(!m_sound->is_playing())
        m_sound->play();

    m_sound->set_position(transformation_data()->position());
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Following_Sound_Module_Stub)

BUILDER_STUB_INITIALIZATION_FUNC(Following_Sound_Module_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_sound(LSound::Sound_Stub::construct_from(sound_stub));
}



Following_Sound_Module_Stub::~Following_Sound_Module_Stub()
{
    delete sound_stub;
}
