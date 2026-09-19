#pragma once

#include <Tools/Mesh_Generation/3D/Mesh_Data_Extractors/Mesh_Data_Extractor.h>


namespace LMD
{

    class Mesh_Data_Extractor__Geometry : public Mesh_Data_Extractor
    {
    public:
        LDS::Vector<float> extract(const Mesh_3D_Utility::Triangles_Vec& _triangles,
                                   const Mesh_3D_Utility::Points_Vec& _points_cache) const override;

    };

}
