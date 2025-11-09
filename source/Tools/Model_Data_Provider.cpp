#include <Tools/Model_Data_Provider.h>

using namespace LMD;


ON_VALUES_ASSIGNED_IMPLEMENTATION(Model_Data_Provider)
{
    L_ASSERT(m_resources_manager);
    L_ASSERT(m_model_name.size() > 0);

    m_model = m_resources_manager->get_resource<Model>(m_model_name);

    if(m_data_extraction_type_str == "Geometry")
        m_extraction_type = Extraction_Type::Geometry;
    else if(m_data_extraction_type_str == "Texture")
        m_extraction_type = Extraction_Type::Texture;
    else if(m_data_extraction_type_str == "Normals")
        m_extraction_type = Extraction_Type::Normals;

    L_ASSERT(m_extraction_type != Extraction_Type::Unknown);
}



const LDS::Vector<float>& Model_Data_Provider::get_data() const
{
    L_ASSERT(m_model);

    if(m_extraction_type == Extraction_Type::Geometry)
        return m_model->geometry_data();
    else if(m_extraction_type == Extraction_Type::Texture)
        return m_model->texture_data();
    else if(m_extraction_type == Extraction_Type::Normals)
        return m_model->normals_data();

    L_ASSERT(false && "unknown model extraction type");

    return m_model->geometry_data();
}
