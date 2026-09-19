#pragma once

#include <Tools/Mesh_Generation/3D/Utility/Mesh_3D_Utilty.h>


namespace LMD
{

    class Mesh_Data_Extractor
    {
    public:
        virtual ~Mesh_Data_Extractor() { }

    public:
        virtual LDS::Vector<float> extract(const Mesh_3D_Utility::Triangles_Vec& _triangles,
                                           const Mesh_3D_Utility::Points_Vec& _points_cache) const = 0;

    };

}
