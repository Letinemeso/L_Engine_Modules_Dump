#include <Tools/Mesh_Generation/3D/Mesh_Data_Extractors/Mesh_Data_Extractor__Normals.h>

#include <Stuff/Math_Stuff.h>

using namespace LMD;


LDS::Vector<float> Mesh_Data_Extractor__Normals::extract(const Mesh_3D_Utility::Triangles_Vec& _triangles,
                                                          const Mesh_3D_Utility::Points_Vec& _points_cache) const
{
    LDS::Vector<float> result(_triangles.size() * 9);

    for(unsigned int t_i = 0; t_i < _triangles.size(); ++t_i)
    {
        const Mesh_3D_Utility::Triangle& triangle = _triangles[t_i];

        const glm::vec3& point_0 = _points_cache[ triangle.id[0] ];
        const glm::vec3& point_1 = _points_cache[ triangle.id[1] ];
        const glm::vec3& point_2 = _points_cache[ triangle.id[2] ];

        glm::vec3 normal = LST::Math::cross_product(point_1 - point_0, point_2 - point_0);
        LST::Math::shrink_vector_to_1(normal);

        for(unsigned int r_i = 0; r_i < 3; ++r_i)
        {
            result.push(normal.x);
            result.push(normal.y);
            result.push(normal.z);
        }
    }

    return result;
}
