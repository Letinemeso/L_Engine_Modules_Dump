#include <Misc_Draw_Modules/Utility_Draw_Modules/Static_Draw_Module.h>

using namespace LMD;


void Static_Draw_Module::set_transformation_data(LEti::Transformation_Data* _data)
{
    Parent_Type::set_transformation_data(&m_initial_transformation);
    if(_data)
    {
        m_initial_transformation.set_rotation(_data->rotation());
        m_initial_transformation.set_scale(_data->scale());
    }
    m_parent_transformation_data = _data;
}

void Static_Draw_Module::set_transformation_data_prev_state(const LEti::Transformation_Data* _data)
{
    Parent_Type::set_transformation_data_prev_state(&m_initial_transformation);
    m_parent_transformation_data_prev_state = _data;
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Static_Draw_Module_Stub)

BUILDER_STUB_INITIALIZATION_FUNC(Static_Draw_Module_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
}
