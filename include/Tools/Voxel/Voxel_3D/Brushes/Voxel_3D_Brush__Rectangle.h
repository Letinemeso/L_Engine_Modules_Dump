#pragma once

#include <Tools/Voxel/Voxel_3D/Brushes/Voxel_3D_Brush.h>


namespace LMD
{

    class Voxel_3D_Brush__Rectangle : public Voxel_3D_Brush
    {
    private:
        glm::vec3 m_offset = {0.0f, 0.0f, 0.0f};
        glm::vec3 m_size = {0.0f, 0.0f, 0.0f};

    public:
        inline void set_offset(const glm::vec3& _value) { m_offset = _value; }
        inline void set_size(const glm::vec3& _value) { m_size = _value; }

        void set_parameters(const glm::vec3& _offset, const glm::vec3& _size);
        void set_parameters_from_center(const glm::vec3& _center, const glm::vec3& _size);

    protected:
        bool M_fully_inside(const LMD::Voxel_3D* _voxel) const override;
        bool M_partially_inside(const LMD::Voxel_3D* _voxel) const override;

    public:
        Border calculate_borders() const override;

    };

}
