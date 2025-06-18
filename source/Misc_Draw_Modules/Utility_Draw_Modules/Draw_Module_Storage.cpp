#include <Misc_Draw_Modules/Utility_Draw_Modules/Draw_Module_Storage.h>

using namespace LMD;


Draw_Module_Storage::Draw_Module_Storage()
{

}

Draw_Module_Storage::~Draw_Module_Storage()
{
    clear_modules();
}



void Draw_Module_Storage::store_modules(Stored_Draw_Modules&& _modules)
{
    clear_modules();
    m_stored_modules = (Stored_Draw_Modules&&)_modules;

    for(Stored_Draw_Modules::Iterator it = m_stored_modules.iterator(); !it.end_reached(); ++it)
    {
        LR::Draw_Module* module = *it;
        module->set_visible(false);
    }

    if(!transformation_data())
        return;

    for(Stored_Draw_Modules::Iterator it = m_stored_modules.iterator(); !it.end_reached(); ++it)
    {
        LR::Draw_Module* module = *it;
        module->set_transformation_data(transformation_data());
        module->set_transformation_data_prev_state(transformation_data_prev_state());
    }
}

void Draw_Module_Storage::store_module(const std::string& _name, LR::Draw_Module* _module)
{
    L_ASSERT(!m_stored_modules.find(_name).is_ok());
    m_stored_modules.insert(_name, _module);

    _module->set_visible(false);

    if(!transformation_data())
        return;

    _module->set_transformation_data(transformation_data());
    _module->set_transformation_data_prev_state(transformation_data_prev_state());
}

void Draw_Module_Storage::clear_modules()
{
    for(Stored_Draw_Modules::Iterator it = m_stored_modules.iterator(); !it.end_reached(); ++it)
        delete *it;
    m_stored_modules.clear();
}


LR::Draw_Module* Draw_Module_Storage::get_module(const std::string& _name) const
{
    Stored_Draw_Modules::Const_Iterator maybe_module_it = m_stored_modules.find(_name);
    L_ASSERT(maybe_module_it.is_ok());
    return *maybe_module_it;
}


void Draw_Module_Storage::switch_to_module(const std::string& _name)
{
    if(m_current_module)
        m_current_module->set_visible(false);

    m_current_module = get_module(_name);
    m_current_module->set_visible(true);
}

void Draw_Module_Storage::reset_selected_module()
{
    if(m_current_module)
        m_current_module->set_visible(false);

    m_current_module = nullptr;
}



void Draw_Module_Storage::M_on_parent_object_set()
{
    for(Stored_Draw_Modules::Iterator it = m_stored_modules.iterator(); !it.end_reached(); ++it)
    {
        LR::Draw_Module* module = *it;
        module->set_transformation_data(transformation_data());
        module->set_transformation_data_prev_state(transformation_data_prev_state());
    }
}



void Draw_Module_Storage::update(float _dt)
{
    if(m_current_module)
        m_current_module->update(_dt);
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Draw_Module_Storage_Stub)

BUILDER_STUB_INITIALIZATION_FUNC(Draw_Module_Storage_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    if(draw_modules_stubs.size() == 0)
        return;

    Draw_Module_Storage::Stored_Draw_Modules modules;
    for(LV::Variable_Base::Childs_List::Const_Iterator it = draw_modules_stubs.begin(); !it.end_reached(); ++it)
    {
        L_ASSERT(LV::cast_variable<LR::Draw_Module_Stub>(it->child_ptr));
        LR::Draw_Module_Stub* stub = (LR::Draw_Module_Stub*)(it->child_ptr);

        modules.insert(it->name, LR::Draw_Module_Stub::construct_from(stub));
    }
    product->store_modules((Draw_Module_Storage::Stored_Draw_Modules&&)modules);

    if(default_module_name.size() > 0)
        product->switch_to_module(default_module_name);
}



Draw_Module_Storage_Stub::~Draw_Module_Storage_Stub()
{
    clear_childs_list(draw_modules_stubs);
}
