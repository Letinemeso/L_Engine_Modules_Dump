#pragma once

#include <Tools/Mesh_Generation/3D/Mesh_Data_Extractors/Mesh_Data_Extractor.h>


namespace LMD
{

    class Mesh_Data_Extractor__Texture : public Mesh_Data_Extractor
    {
    public:
        INIT_VARIABLE(LMD::Mesh_Data_Extractor__Texture, LMD::Mesh_Data_Extractor)

    private:
        float m_extraction_cell_size = 0.0f;

    public:
        inline void set_extraction_cell_size(float _value) { m_extraction_cell_size = _value; }

    public:
        LDS::Vector<float> extract(const Mesh_3D_Utility::Triangles_Vec& _triangles,
                                   const Mesh_3D_Utility::Points_Vec& _points_cache) const override;

    };


    class Mesh_Data_Extractor_Stub__Texture : public Mesh_Data_Extractor_Stub
    {
    public:
        INIT_VARIABLE(LMD::Mesh_Data_Extractor_Stub__Texture, LMD::Mesh_Data_Extractor_Stub)

        INIT_FIELDS
        ADD_FIELD(float, extraction_cell_size)
        ADD_FIELD(float, voxel_size)
        ADD_FIELD(unsigned int, max_extraction_depth)
        FIELDS_END

    public:
        float extraction_cell_size = -1.0f;     //  this is used if defined explicitly

        float voxel_size = 0.0f;                //  if not, these 2 values are used to calculate extraction_cell_size
        unsigned int max_extraction_depth = 0;

    public:
        INIT_DEFAULT_BUILDER_STUB(Mesh_Data_Extractor__Texture)

        INIT_BUILDER_STUB_SETTERS
        ADD_BUILDER_STUB_SETTER(set_extraction_cell_size, M_calculate_extraction_cell_size())
        BUILDER_STUB_SETTERS_END

    private:
        float M_calculate_extraction_cell_size() const;

    };

}
