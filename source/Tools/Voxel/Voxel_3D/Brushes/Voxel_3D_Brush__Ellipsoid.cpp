#include <Tools/Voxel/Voxel_3D/Brushes/Voxel_3D_Brush__Ellipsoid.h>

#include <Stuff/Math_Stuff.h>

using namespace LMD;


bool Voxel_3D_Brush__Ellipsoid::M_point_is_inside(const glm::vec3& _point) const
{
    glm::vec3 delta = (_point - m_center) / m_radiuses;
    return LST::Math::dot_product(delta, delta) <= 1.0f;
}



bool Voxel_3D_Brush__Ellipsoid::M_fully_inside(const LMD::Voxel_3D* _voxel) const
{
    const glm::vec3& voxel_offset = _voxel->offset();
    const float voxel_size = _voxel->size();

    for(int x = 0; x <= 1; ++x)
    {
        for(int y = 0; y <= 1; ++y)
        {
            for(int z = 0; z <= 1; ++z)
            {
                glm::vec3 vertex = voxel_offset + glm::vec3(x * voxel_size, y * voxel_size, z * voxel_size );

                if(!M_point_is_inside(vertex))
                    return false;
            }
        }
    }

    return true;
}

bool Voxel_3D_Brush__Ellipsoid::M_partially_inside(const LMD::Voxel_3D* _voxel) const
{
    float voxel_size = _voxel->size();

    glm::vec3 min = _voxel->offset();
    glm::vec3 max = min + glm::vec3(voxel_size, voxel_size, voxel_size);

    glm::vec3 closest_point_in_voxel = glm::clamp(m_center, min, max);

    return M_point_is_inside(closest_point_in_voxel);
}
