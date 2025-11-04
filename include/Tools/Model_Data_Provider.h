#pragma once

#include <Data_Provider.h>
#include <Resources_Manager.h>

#include <Resources/Model.h>


namespace LMD
{

    class Model_Data_Provider : public LEti::Data_Provider
    {
    public:
        INIT_VARIABLE(LMD::Model_Data_Provider, LEti::Data_Provider)

        INIT_FIELDS
        ADD_FIELD_RENAMED(std::string, "model_name", m_model_name)
        ADD_FIELD_RENAMED(std::string, "data_extraction_type", m_data_extraction_type_str)
        FIELDS_END

        OVERRIDE_ON_VALUES_ASSIGNED

    private:
        enum class Extraction_Type
        {
            Unknown,
            Geometry,
            Texture
        };

    private:
        std::string m_model_name;
        std::string m_data_extraction_type_str;

        const LEti::Resources_Manager* m_resources_manager = nullptr;

    private:
        const Model* m_model = nullptr;
        Extraction_Type m_extraction_type = Extraction_Type::Unknown;

    public:
        inline void inject_resources_manager(const LEti::Resources_Manager* _ptr) { m_resources_manager = _ptr; }

    public:
        const LDS::Vector<float>& get_data() const override;

    };

}
