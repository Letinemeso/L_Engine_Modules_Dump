#include <Tools/Objects_Controller/Objects_Controller.h>

using namespace LMD;


Objects_Controller::Objects_Controller()
{

}

Objects_Controller::~Objects_Controller()
{
    clear_objects();
}



LEti::Object* Objects_Controller::get_suitable_object(const LST::Function<bool(LEti::Object*)>& _condition) const
{
    L_ASSERT(_condition);

    for(Objects_List::Const_Iterator it = m_objects.begin(); !it.end_reached(); ++it)
    {
        LEti::Object* entity = *it;

        if(_condition(entity))
            return entity;
    }

    return nullptr;
}



void Objects_Controller::add_object(LEti::Object* _object)
{
    L_ASSERT(_object);
    L_ASSERT(!m_objects.find(_object).is_ok());

    m_objects.push_back(_object);

    for(unsigned int i = 0; i < m_extensions.size(); ++i)
        m_extensions[i]->on_object_added(_object);
}

void Objects_Controller::remove_object(LEti::Object* _object)
{
    Objects_List::Iterator obj_it = m_objects.find(_object);
    L_ASSERT(obj_it.is_ok());
    m_objects.erase(obj_it);
}

void Objects_Controller::clear_objects()
{
    for(Objects_List::Iterator it = m_objects.begin(); !it.end_reached(); ++it)
        delete *it;
    m_objects.clear();
}

void Objects_Controller::clear_extensions()
{
    for(unsigned int i = 0; i < m_extensions.size(); ++i)
        delete m_extensions[i];
    m_extensions.mark_empty();
}



bool Objects_Controller::M_need_to_remove_object(LEti::Object* _object) const
{
    for(unsigned int i = 0; i < m_extensions.size(); ++i)
    {
        if(m_extensions[i]->should_remove_object(_object))
            return true;
    }

    return false;
}

void Objects_Controller::M_remove_objects()
{
    Objects_List::Iterator it = m_objects.begin();
    while(!it.end_reached())
    {
        LEti::Object* object = *it;
        if(!M_need_to_remove_object(object))
        {
            ++it;
            continue;
        }

        for(unsigned int i = 0; i < m_extensions.size(); ++i)
            m_extensions[i]->on_object_removed(object);

        delete object;
        it = m_objects.erase_and_iterate_forward(it);
    }
}



void Objects_Controller::update_previous_state()
{
    for(Objects_List::Iterator it = m_objects.begin(); !it.end_reached(); ++it)
    {
        LEti::Object* object = *it;
        object->update_previous_state();
    }
}

void Objects_Controller::update(float _dt)
{
    for(Objects_List::Iterator it = m_objects.begin(); !it.end_reached(); ++it)
    {
        LEti::Object* object = *it;
        object->update(_dt);
    }

    for(unsigned int i = 0; i < m_extensions.size(); ++i)
        m_extensions[i]->update(m_objects, _dt);

    M_remove_objects();

    for(unsigned int i = 0; i < m_extensions.size(); ++i)
        m_extensions[i]->reset();
}
