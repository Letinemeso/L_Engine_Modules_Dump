#include <Tools/Owned_Model_Data_Provider.h>

#include <Object.h>

#include <Misc_Modules/Data_Storage_Module.h>

using namespace LMD;


ON_VALUES_ASSIGNED_IMPLEMENTATION(Owned_Model_Data_Provider)
{
    L_ASSERT(m_data_name.size() > 0);

    if(m_data_extraction_type_str == "Geometry")
        m_extraction_type = Extraction_Type::Geometry;
    else if(m_data_extraction_type_str == "Texture")
        m_extraction_type = Extraction_Type::Texture;
    else if(m_data_extraction_type_str == "Normals")
        m_extraction_type = Extraction_Type::Normals;

    L_ASSERT(m_extraction_type != Extraction_Type::Unknown);
}



const LMD::Model* Owned_Model_Data_Provider::M_find_model(const LEti::Object* _future_parent) const
{
    L_ASSERT(_future_parent);

    const LMD::Model* result = nullptr;

    _future_parent->process_logic_for_modules_of_type<LMD::Data_Storage_Module>([this, &result](const LMD::Data_Storage_Module* _module)
    {
        if(_module->data_name() != m_data_name)
            return;

        const LMD::Model* maybe_model = LV::cast_variable<LMD::Model>(_module->data());
        if(maybe_model)
            result = maybe_model;
    });

    L_ASSERT(result);
    return result;
}



const LDS::Vector<float>& Owned_Model_Data_Provider::get_data(const LEti::Object* _future_parent) const
{
    const LMD::Model* model = M_find_model(_future_parent);

    if(m_extraction_type == Extraction_Type::Geometry)
        return model->geometry_data();
    else if(m_extraction_type == Extraction_Type::Texture)
        return model->texture_data();
    else if(m_extraction_type == Extraction_Type::Normals)
        return model->normals_data();

    L_ASSERT_WITH_INFO(false, "unknown model extraction type");

    return model->geometry_data();
}
