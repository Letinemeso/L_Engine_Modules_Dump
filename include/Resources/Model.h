#pragma once

#include <Data_Structures/Vector.h>

#include <Variable_Base.h>
#include <Builder_Stub.h>


namespace LMD
{

    class Model : public LV::Variable_Base
    {
    public:
        INIT_VARIABLE(LMD::Model, LV::Variable_Base)

    private:
        LDS::Vector<float> m_geometry_data;
        LDS::Vector<float> m_texture_data;

    public:
        void set_geometry_data(const LDS::Vector<float>& _data);
        void set_geometry_data(LDS::Vector<float>&& _data);
        void set_texture_data(const LDS::Vector<float>& _data);
        void set_texture_data(LDS::Vector<float>&& _data);

    public:
        const LDS::Vector<float>& geometry_data() const { return m_geometry_data; }
        const LDS::Vector<float>& texture_data() const { return m_texture_data; }

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
        LDS::Vector<float> geometry_data;
        LDS::Vector<float> texture_data;

    public:
        INIT_BUILDER_STUB(Model)

    };

}
