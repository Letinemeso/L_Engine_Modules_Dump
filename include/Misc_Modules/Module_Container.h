#pragma once

#include <Data_Structures/List.h>

#include <Object.h>
#include <Module.h>


namespace LMD
{

    class Module_Container : public LEti::Module
    {
    public:
        INIT_VARIABLE(LMD::Module_Container, LEti::Module)

    private:
        LEti::Object::Modules_List m_modules;

    public:
        Module_Container();
        ~Module_Container();

    public:
        void add_module(Module* _module, bool _push_back = true);
        void remove_module(Module* _module);
        void remove_all_modules();

    public:
        inline const LEti::Object::Modules_List& modules() const { return m_modules; }

    public:
        template<typename Module_Type>
        void process_logic_for_modules_of_type(const LST::Function<void(Module_Type*)>& _logic);
        template<typename Module_Type>
        void process_logic_for_modules_of_type(const LST::Function<void(const Module_Type*)>& _logic) const;
        template<typename Module_Type>
        Module_Type* get_module_of_type(unsigned int _skip_amount = 0) const;

    protected:
        void M_on_parent_object_set() override;

    public:
        void update_prev_state() override;
        void update(float _dt) override;

    };

    template<typename Module_Type>
    void Module_Container::process_logic_for_modules_of_type(const LST::Function<void(Module_Type*)>& _logic)
    {
        L_ASSERT(_logic);

        for(LEti::Object::Modules_List::Iterator module_it = m_modules.begin(); !module_it.end_reached(); ++module_it)
        {
            Module_Type* requested_type_module = LV::cast_variable<Module_Type>(*module_it);
            if(!requested_type_module)
                continue;

            _logic(requested_type_module);
        }
    }

    template<typename Module_Type>
    void Module_Container::process_logic_for_modules_of_type(const LST::Function<void(const Module_Type*)>& _logic) const
    {
        L_ASSERT(_logic);

        for(LEti::Object::Modules_List::Const_Iterator module_it = m_modules.begin(); !module_it.end_reached(); ++module_it)
        {
            const Module_Type* requested_type_module = LV::cast_variable<Module_Type>(*module_it);
            if(!requested_type_module)
                continue;

            _logic(requested_type_module);
        }
    }

    template<typename Module_Type>
    Module_Type* Module_Container::get_module_of_type(unsigned int _skip_amount) const
    {
        for(LEti::Object::Modules_List::Const_Iterator module_it = m_modules.begin(); !module_it.end_reached(); ++module_it)
        {
            Module_Type* requested_type_module = LV::cast_variable<Module_Type>(*module_it);
            if(!requested_type_module)
                continue;

            if(_skip_amount == 0)
                return requested_type_module;

            --_skip_amount;
        }

        return nullptr;
    }


    class Module_Container_Stub : public LEti::Module_Stub
    {
    public:
        INIT_VARIABLE(LMD::Module_Container_Stub, LEti::Module_Stub)

        INIT_CHILDS_LISTS
        ADD_CHILDS_LIST("Module__*", &modules)
        CHILDS_LISTS_END

    public:
        LV::Variable_Base::Childs_List modules;

    public:
        INIT_BUILDER_STUB(Module_Container)

    public:
        ~Module_Container_Stub();

    };

}
