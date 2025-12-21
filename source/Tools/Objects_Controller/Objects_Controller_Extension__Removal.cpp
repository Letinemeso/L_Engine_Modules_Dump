#include <Tools/Objects_Controller/Objects_Controller_Extension__Removal.h>

using namespace LMD;


Objects_Controller_Extension__Removal::Objects_Controller_Extension__Removal()
{
    m_message_handle = LST::Message_Translator::instance().subscribe<Message__Request_Entity_Death>([this](Message__Request_Entity_Death& _msg)
    {
        if(m_should_be_dead_entities.find(_msg.should_be_dead_entity).is_ok())
            return;

        m_should_be_dead_entities.insert(_msg.should_be_dead_entity);
    });
}

Objects_Controller_Extension__Removal::~Objects_Controller_Extension__Removal()
{
    LST::Message_Translator::instance().unsubscribe(m_message_handle);
}



void Objects_Controller_Extension__Removal::reset()
{
    m_should_be_dead_entities.clear();
}

bool Objects_Controller_Extension__Removal::should_remove_object(LEti::Object* _object)
{
    return m_should_be_dead_entities.find(_object).is_ok();
}
