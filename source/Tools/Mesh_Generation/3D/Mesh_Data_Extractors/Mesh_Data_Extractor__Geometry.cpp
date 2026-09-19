#include <Tools/Mesh_Generation/3D/Mesh_Data_Extractors/Mesh_Data_Extractor__Geometry.h>

using namespace LMD;


LDS::Vector<float> Mesh_Data_Extractor__Geometry::extract(const Mesh_3D_Utility::Triangles_Vec& _triangles,
                                                          const Mesh_3D_Utility::Points_Vec& _points_cache) const
{
    LDS::Vector<float> result(_triangles.size() * 9);

    for(unsigned int t_i = 0; t_i < _triangles.size(); ++t_i)
    {
        const Mesh_3D_Utility::Triangle& triangle = _triangles[t_i];
        for(unsigned int i = 0; i < 3; ++i)
        {
            const glm::vec3& vec = _points_cache[ triangle.id[i] ];

            for(unsigned int v_i = 0; v_i < 3; ++v_i)
                result.push( vec[v_i] );
        }
    }

    return result;
}
