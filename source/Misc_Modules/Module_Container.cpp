#include <Misc_Modules/Module_Container.h>

using namespace LMD;


Module_Container::Module_Container()
{

}

Module_Container::~Module_Container()
{
    remove_all_modules();
}



void Module_Container::add_module(LEti::Module* _module, bool _push_back)
{
    L_ASSERT(_module);
    L_DEBUG_FUNC_NOARG([&]()
    {
        for(unsigned int i = 0; i < m_modules.size(); ++i)
        {
            L_ASSERT(m_modules[i] != _module);
        }
    });

    if(_push_back)
        m_modules.push(_module);
    else
        m_modules.push_front(_module);

    if(!parent_object())
        return;

    _module->set_transformation_data(transformation_data());
    _module->set_transformation_data_prev_state(transformation_data_prev_state());
    _module->set_parent_object(parent_object());
}

void Module_Container::remove_module(LEti::Module* _module)
{
    L_ASSERT(_module);

    unsigned int index = 0;
    for(; index < m_modules.size(); ++index)
    {
        if(m_modules[index] == _module)
            break;
    }

    L_ASSERT(index < m_modules.size());

    _module->set_transformation_data(nullptr);
    _module->set_transformation_data_prev_state(nullptr);
    _module->set_parent_object(nullptr);

    m_modules.pop(index);
}

void Module_Container::remove_all_modules()
{
    for(unsigned int i = 0; i < m_modules.size(); ++i)
        delete m_modules[i];

    m_modules.mark_empty();
}



void Module_Container::M_on_parent_object_set()
{
    for(unsigned int i = 0; i < m_modules.size(); ++i)
    {
        LEti::Module* module = m_modules[i];

        module->set_transformation_data(transformation_data());
        module->set_transformation_data_prev_state(transformation_data_prev_state());
        module->set_parent_object(parent_object());
    }
}



void Module_Container::update_prev_state()
{
    for(unsigned int i = 0; i < m_modules.size(); ++i)
        m_modules[i]->update_prev_state();
}

void Module_Container::update(float _dt)
{
    for(unsigned int i = 0; i < m_modules.size(); ++i)
        m_modules[i]->update(_dt);
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Module_Container_Stub)

BUILDER_STUB_INITIALIZATION_FUNC(Module_Container_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    for(LV::Variable_Base::Childs_List::Const_Iterator it = modules.begin(); !it.end_reached(); ++it)
    {
        LEti::Module_Stub* stub = (LEti::Module_Stub*)it->child_ptr;
        L_ASSERT(LV::cast_variable<LEti::Module_Stub>(stub));

        LEti::Module* module = LEti::Module_Stub::construct_from(stub);

        product->add_module(module);
    }
}



Module_Container_Stub::~Module_Container_Stub()
{
    clear_childs_list(modules);
}
