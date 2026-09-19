#include <Tools/Mesh_Generation/3D/Mesh_Data_Extractors/Mesh_Data_Extractor__Texture.h>

#include <Stuff/Math_Stuff.h>

using namespace LMD;


LDS::Vector<float> Mesh_Data_Extractor__Texture::extract(const Mesh_3D_Utility::Triangles_Vec& _triangles,
                                                          const Mesh_3D_Utility::Points_Vec& _points_cache) const
{
    LDS::Vector<float> result(_triangles.size() * 6);

    for(unsigned int t_i = 0; t_i < _triangles.size(); ++t_i)
    {
        const Mesh_3D_Utility::Triangle& triangle = _triangles[t_i];

        const glm::vec3& point_0 = _points_cache[ triangle.id[0] ];
        const glm::vec3& point_1 = _points_cache[ triangle.id[1] ];
        const glm::vec3& point_2 = _points_cache[ triangle.id[2] ];

        glm::vec3 triangle_normal = LST::Math::cross_product(point_1 - point_0, point_2 - point_0);
        LST::Math::shrink_vector_to_1(triangle_normal);

        glm::vec3 u_axis = point_1 - point_0;
        LST::Math::shrink_vector_to_1(u_axis);

        glm::vec3 v_axis = LST::Math::cross_product(triangle_normal, u_axis);
        LST::Math::shrink_vector_to_1(v_axis);

        result.push(0.0f);
        result.push(0.0f);

        result.push( LST::Math::dot_product(point_1 - point_0, u_axis) / m_extraction_cell_size );
        result.push(0.0f);

        result.push( LST::Math::dot_product(point_2 - point_0, u_axis) / m_extraction_cell_size );
        result.push( LST::Math::dot_product(point_2 - point_0, v_axis) / m_extraction_cell_size );
    }

    return result;
}
