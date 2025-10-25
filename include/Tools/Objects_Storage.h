#pragma once

#include <Data_Structures/Map.h>

#include <Variable_Base.h>


namespace LMD
{

    class Objects_Storage : public LV::Variable_Base
    {
    public:
        INIT_VARIABLE(LMD::Objects_Storage, LV::Variable_Base)

        INIT_CHILDS_LISTS
        ADD_CHILDS_LIST("*", &m_childs)
        CHILDS_LISTS_END

        OVERRIDE_ON_VALUES_ASSIGNED

    private:
        LV::Variable_Base::Childs_List m_childs;

    private:
        using Objects_Map = LDS::Map<std::string, LV::Variable_Base*>;
        Objects_Map m_objects;

    public:
        Objects_Storage();
        ~Objects_Storage();

    public:
        LV::Variable_Base* get_object(const std::string& _id) const;

        template<typename _Cast_To_Type>
        _Cast_To_Type* get_object(const std::string& _id) const;

    };


    template<typename _Cast_To_Type>
    _Cast_To_Type* Objects_Storage::get_object(const std::string& _id) const
    {
        LV::Variable_Base* object = get_object(_id);
        if(!object)
            return nullptr;

        L_ASSERT(LV::cast_variable<_Cast_To_Type>(object));

        return (_Cast_To_Type*)object;
    }

}
