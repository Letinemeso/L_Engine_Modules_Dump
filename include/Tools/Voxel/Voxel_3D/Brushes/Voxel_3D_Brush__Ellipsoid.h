#pragma once

#include <Tools/Voxel/Voxel_3D/Brushes/Voxel_3D_Brush.h>


namespace LMD
{

class Voxel_3D_Brush__Ellipsoid : public Voxel_3D_Brush
{
private:
    glm::vec3 m_center = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_radiuses = {0.0f, 0.0f, 0.0f};

public:
    inline void set_center(const glm::vec3& _value) { m_center = _value; }
    inline void set_radiuses(const glm::vec3& _value) { m_radiuses = _value; }

protected:
    bool M_point_is_inside(const glm::vec3& _point) const;

    bool M_fully_inside(const LMD::Voxel_3D* _voxel) const override;
    bool M_partially_inside(const LMD::Voxel_3D* _voxel) const override;

};

}
