#pragma once

#include <Variable_Base.h>
#include <Builder_Stub.h>


namespace LMD
{

    class Model : public LV::Variable_Base
    {
    public:
        INIT_VARIABLE(LMD::Model, LV::Variable_Base)

    private:
        float* m_geometry_data = nullptr;
        unsigned int m_geometry_data_size = 0;
        float* m_texture_data = nullptr;
        unsigned int m_texture_data_size = 0;

    public:
        Model();
        ~Model();

    public:
        void copy_geometry_data(const float* _raw_data, unsigned int _raw_data_size);
        void use_geometry_data(float* _raw_data, unsigned int _raw_data_size);
        void copy_texture_data(const float* _raw_data, unsigned int _raw_data_size);
        void use_texture_data(float* _raw_data, unsigned int _raw_data_size);

    public:
        const float* geometry_data() const { return m_geometry_data; }
        unsigned int geometry_data_size() const { return m_geometry_data_size; }
        const float* texture_data() const { return m_texture_data; }
        unsigned int texture_data_size() const { return m_texture_data_size; }

    };


    class Model_Stub : public LV::Builder_Stub
    {
    public:
        INIT_VARIABLE(LMD::Model_Stub, LV::Builder_Stub)

        INIT_FIELDS
        ADD_FIELD(std::string, file_path)
        FIELDS_END

        OVERRIDE_ON_VALUES_ASSIGNED

    public:
        std::string file_path;

    public:
        float* geometry_data = nullptr;
        unsigned int geometry_data_size = 0;
        float* texture_data = nullptr;
        unsigned int texture_data_size = 0;

    public:
        ~Model_Stub();

    public:
        INIT_BUILDER_STUB(Model)

    };

}
