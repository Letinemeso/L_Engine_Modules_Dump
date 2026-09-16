#include <Tools/Voxel/Voxel_3D/Brushes/Voxel_3D_Brush__Rectangle.h>

#include <Stuff/Math_Stuff.h>

using namespace LMD;


void Voxel_3D_Brush__Rectangle::set_parameters(const glm::vec3& _offset, const glm::vec3& _size)
{
    set_offset(_offset);
    set_size(_size);
}

void Voxel_3D_Brush__Rectangle::set_parameters_from_center(const glm::vec3& _center, const glm::vec3& _size)
{
    set_offset(_center - (_size * 0.5f));
    set_size(_size);
}



bool Voxel_3D_Brush__Rectangle::M_fully_inside(const LMD::Voxel_3D* _voxel) const
{
    const glm::vec3& voxel_offset = _voxel->offset();

    for(unsigned int i = 0; i < 3; ++i)
    {
        float offset_component = m_offset[i];
        float limit_component = m_offset[i] + m_size[i];

        if(voxel_offset[i] < offset_component)
            return false;
        if(voxel_offset[i] > limit_component)
            return false;

        float component_limit = voxel_offset[i] + _voxel->size();

        if(component_limit < offset_component)
            return false;
        if(component_limit > limit_component)
            return false;
    }

    return true;
}

bool Voxel_3D_Brush__Rectangle::M_partially_inside(const LMD::Voxel_3D* _voxel) const
{
    const glm::vec3& voxel_offset = _voxel->offset();

    for(unsigned int i = 0; i < 3; ++i)
    {
        float this_min = m_offset[i];
        float this_max = m_offset[i] + m_size[i];

        float voxel_min = voxel_offset[i];
        float voxel_max = voxel_offset[i] + _voxel->size();

        if(!(voxel_min < this_max && voxel_max > this_min))
            return false;
    }

    return true;
}



Voxel_3D_Brush::Border Voxel_3D_Brush__Rectangle::calculate_borders() const
{
    Border result;
    result.offset = m_offset;
    result.limit = m_offset + m_size;
    return result;
}
