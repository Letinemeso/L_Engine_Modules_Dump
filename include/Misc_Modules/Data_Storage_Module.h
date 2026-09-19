#pragma once

#include <Variable_Base.h>
#include <Builder_Stub.h>

#include <Module.h>


namespace LMD
{

    class Data_Storage_Module : public LEti::Module
    {
    public:
        INIT_VARIABLE(LMD::Data_Storage_Module, LEti::Module)

    private:
        std::string m_data_name;
        LV::Variable_Base* m_data = nullptr;

    public:
        Data_Storage_Module();
        ~Data_Storage_Module();

    public:
        inline void set_data_name(const std::string& _value) { m_data_name = _value; }
        inline void set_data(LV::Variable_Base* _ptr) { delete m_data; m_data = _ptr; }

        inline const std::string& data_name() const { return m_data_name; }
        inline LV::Variable_Base* data() const { return m_data; }

    public:
        void update(float _dt) override { }

    };


    class Data_Storage_Module_Stub : public LEti::Module_Stub
    {
    public:
        INIT_VARIABLE(LMD::Data_Storage_Module_Stub, LEti::Module_Stub)

        INIT_FIELDS
        ADD_FIELD(std::string, data_name)
        FIELDS_END

        INIT_CHILDS
        ADD_CHILD("data_stub", data_stub)
        CHILDS_END

    public:
        std::string data_name;

        LV::Builder_Stub* data_stub = nullptr;

    public:
        INIT_DEFAULT_BUILDER_STUB(Data_Storage_Module)

        INIT_BUILDER_STUB_SETTERS
        ADD_BUILDER_STUB_SETTER(set_data_name, data_name)
        ADD_BUILDER_STUB_SETTER(set_data, data_stub ? data_stub->construct() : nullptr)
        BUILDER_STUB_SETTERS_END

    public:
        ~Data_Storage_Module_Stub();

    };

}
