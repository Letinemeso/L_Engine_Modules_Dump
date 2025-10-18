#pragma once

#include <Data_Structures/Map.h>

#include <Draw_Modules/Draw_Module.h>


namespace LMD
{

    class Draw_Module_Storage : public LEti::Module
    {
    public:
        INIT_VARIABLE(LMD::Draw_Module_Storage, LEti::Module)

    public:
        using Stored_Draw_Modules = LDS::Map<std::string, LR::Draw_Module*>;

    private:
        Stored_Draw_Modules m_stored_modules;
        LR::Draw_Module* m_current_module = nullptr;

    public:
        Draw_Module_Storage();
        ~Draw_Module_Storage();

    public:
        void store_modules(Stored_Draw_Modules&& _modules);
        void store_module(const std::string& _name, LR::Draw_Module* _module);
        void clear_modules();

        LR::Draw_Module* get_module(const std::string& _name) const;

        void switch_to_module(const std::string& _name);
        void reset_selected_module();

    public:
        inline LR::Draw_Module* current_module() const { return m_current_module; }

    protected:
        void M_on_parent_object_set() override;

    public:
        void update(float _dt) override;

    };


    class Draw_Module_Storage_Stub : public LEti::Module_Stub
    {
    public:
        INIT_VARIABLE(LMD::Draw_Module_Storage_Stub, LEti::Module_Stub)

        INIT_FIELDS
        ADD_FIELD(std::string, default_module_name)
        FIELDS_END

        INIT_CHILDS_LISTS
        ADD_CHILDS_LIST("Draw_Module__*", &draw_modules_stubs)
        CHILDS_LISTS_END

    public:
        LV::Variable_Base::Childs_List draw_modules_stubs;
        std::string default_module_name;

    public:
        INIT_BUILDER_STUB(Draw_Module_Storage)

    public:
        ~Draw_Module_Storage_Stub();

    };

}
