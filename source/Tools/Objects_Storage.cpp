#include <Tools/Objects_Storage.h>

using namespace LMD;


ON_VALUES_ASSIGNED_IMPLEMENTATION(Objects_Storage)
{
    for(LV::Variable_Base::Childs_List::Iterator it = m_childs.begin(); !it.end_reached(); ++it)
        m_objects.insert(it->name, it->child_ptr);

    m_childs.clear();
}



Objects_Storage::Objects_Storage()
{

}

Objects_Storage::~Objects_Storage()
{
    for(Objects_Map::Iterator it = m_objects.iterator(); !it.end_reached(); ++it)
        delete *it;
}



LV::Variable_Base* Objects_Storage::get_object(const std::string& _id) const
{
    Objects_Map::Const_Iterator maybe_object_it = m_objects.find(_id);
    if(!maybe_object_it.is_ok())
        return nullptr;

    return *maybe_object_it;
}
