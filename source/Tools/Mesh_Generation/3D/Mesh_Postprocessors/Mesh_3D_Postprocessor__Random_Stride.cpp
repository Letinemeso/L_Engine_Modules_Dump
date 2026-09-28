#include <Tools/Mesh_Generation/3D/Mesh_Postprocessors/Mesh_3D_Postprocessor__Random_Stride.h>

#include <Stuff/Math_Stuff.h>

using namespace LMD;


void Mesh_3D_Postprocessor__Random_Stride::apply(Mesh_3D_Utility::Points_Vec& _points,
                                                 const Mesh_3D_Utility::Voxel_Triangles_Map& _mesh_structure) const
{
    for(unsigned int i = 0; i < _points.size(); ++i)
    {
        float random_stride_magnitude = LST::Math::random_number_float(m_min_stride, m_max_stride);
        glm::vec3 random_stride = LST::Math::random_vec3(random_stride_magnitude);

        // glm::vec3 random_stride = LST::Math::random_vec3({m_min_stride, m_min_stride, m_min_stride}, {m_max_stride, m_max_stride, m_max_stride});

        _points[i] += random_stride;
    }
}
