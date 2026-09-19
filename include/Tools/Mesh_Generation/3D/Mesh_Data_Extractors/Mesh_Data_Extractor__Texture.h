#pragma once

#include <Tools/Mesh_Generation/3D/Mesh_Data_Extractors/Mesh_Data_Extractor.h>


namespace LMD
{

class Mesh_Data_Extractor__Texture : public Mesh_Data_Extractor
{
private:
    float m_extraction_cell_size = 0.0f;

public:
    inline void set_extraction_cell_size(float _value) { m_extraction_cell_size = _value; }

public:
    LDS::Vector<float> extract(const Mesh_3D_Utility::Triangles_Vec& _triangles,
                               const Mesh_3D_Utility::Points_Vec& _points_cache) const override;

};

}
