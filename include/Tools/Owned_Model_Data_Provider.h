#pragma once

#include <Data_Provider.h>
#include <Variable_Loader.h>

#include <Resources/Model.h>


namespace LMD
{

    class Owned_Model_Data_Provider : public LEti::Data_Provider
    {
    public:
        INIT_VARIABLE(LMD::Owned_Model_Data_Provider, LEti::Data_Provider)

        INIT_FIELDS
        ADD_FIELD_RENAMED(std::string, "data_name", m_data_name)
        ADD_FIELD_RENAMED(std::string, "data_extraction_type", m_data_extraction_type_str)
        FIELDS_END

        OVERRIDE_ON_VALUES_ASSIGNED

    private:
        enum class Extraction_Type
        {
            Unknown,
            Geometry,
            Texture,
            Normals
        };

    private:
        std::string m_data_name;
        std::string m_data_extraction_type_str;

    private:
        Extraction_Type m_extraction_type = Extraction_Type::Unknown;

    private:
        const LMD::Model* M_find_model(const LEti::Object* _future_parent) const;

    public:
        const LDS::Vector<float>& get_data(const LEti::Object* _future_parent) const override;

    };

}
