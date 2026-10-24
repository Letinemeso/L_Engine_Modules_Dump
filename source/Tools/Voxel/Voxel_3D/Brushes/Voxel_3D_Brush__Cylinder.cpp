#include <Tools/Voxel/Voxel_3D/Brushes/Voxel_3D_Brush__Cylinder.h>

#include <Stuff/Math_Stuff.h>

using namespace LMD;


bool Voxel_3D_Brush__Cylinder::M_point_is_inside(const glm::vec3& _point) const
{
    if(!LST::Math::value_is_between(m_center.y - m_radiuses.y, m_center.y + m_radiuses.y, _point.y))
        return false;

    return M_point_is_inside_flat_ellipse({_point.x, _point.z});
}

bool Voxel_3D_Brush__Cylinder::M_point_is_inside_flat_ellipse(const glm::vec2& _point) const
{
    float offset_x = (_point.x - m_center.x) / m_radiuses.x;
    float offset_z = (_point.y - m_center.z) / m_radiuses.z;

    return offset_x * offset_x + offset_z * offset_z <= 1.0 + LST::Math::Float_Precision_Tolerance;
}



bool Voxel_3D_Brush__Cylinder::M_fully_inside(const LMD::Voxel_3D* _voxel) const
{
    const glm::vec3& voxel_offset = _voxel->offset();
    const float voxel_size = _voxel->size();

    float vertical_offset = m_center.y - m_radiuses.y;
    float vertical_limit = m_center.y + m_radiuses.y;

    if(!LST::Math::value_is_between(vertical_offset, vertical_limit, voxel_offset.y))
        return false;
    if(!LST::Math::value_is_between(vertical_offset, vertical_limit, voxel_offset.y + voxel_size))
        return false;

    for(int x = 0; x <= 1; ++x)
    {
        for(int z = 0; z <= 1; ++z)
        {
            glm::vec2 vertex = glm::vec2(voxel_offset.x, voxel_offset.z) + glm::vec2(x * voxel_size, z * voxel_size );

            if(!M_point_is_inside_flat_ellipse(vertex))
                return false;
        }
    }

    return true;
}

bool Voxel_3D_Brush__Cylinder::M_partially_inside(const LMD::Voxel_3D* _voxel) const
{
    float voxel_size = _voxel->size();

    glm::vec3 min = _voxel->offset();
    glm::vec3 max = min + glm::vec3(voxel_size, voxel_size, voxel_size);

    glm::vec3 closest_point_in_voxel = glm::clamp(m_center, min, max);

    return M_point_is_inside(closest_point_in_voxel);
}



Voxel_3D_Brush::Border Voxel_3D_Brush__Cylinder::calculate_borders() const
{
    Border result;
    result.offset = m_center - m_radiuses;
    result.limit = m_center + m_radiuses;
    return result;
}
