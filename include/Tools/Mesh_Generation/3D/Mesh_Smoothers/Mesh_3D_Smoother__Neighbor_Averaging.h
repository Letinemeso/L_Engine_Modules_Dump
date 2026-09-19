#pragma once

#include <Tools/Mesh_Generation/3D/Mesh_Smoothers/Mesh_3D_Smoother.h>


namespace LMD
{

    class Mesh_3D_Smoother__Neighbor_Averaging : public Mesh_3D_Smoother
    {
    private:
        float m_smooth_factor = 1.0f;
        unsigned int m_repetitions_amount = 1;

    public:
        inline void set_smooth_factor(float _value) { L_ASSERT(_value > 0.0f && _value < 1.00001f); m_smooth_factor = _value; }
        inline void set_repetitions_amount(unsigned int _value) { L_ASSERT(_value > 0); m_repetitions_amount = _value; }

    private:
        void M_append_points_neighbors_for_triangle(Mesh_3D_Utility::Point_Neighbors_Vec& _neighbors, const Mesh_3D_Utility::Triangle& _triangle) const;
        Mesh_3D_Utility::Point_Neighbors_Vec M_update_point_neighbors(const Mesh_3D_Utility::Voxel_Triangles_Map& _mesh_structure, unsigned int _points_amount) const;

        glm::vec3 M_smooth_point(const glm::vec3& _point, const Mesh_3D_Utility::Points_Vec& _old_mesh, const Mesh_3D_Utility::IDs_Vec& _neighbors_ids) const;
        Mesh_3D_Utility::Points_Vec M_smooth(const Mesh_3D_Utility::Points_Vec& _old_mesh, const Mesh_3D_Utility::Point_Neighbors_Vec& _points_neighbors) const;

    public:
        Mesh_3D_Utility::Points_Vec smooth(const Mesh_3D_Utility::Points_Vec& _old_mesh,
                                           const Mesh_3D_Utility::Voxel_Triangles_Map& _mesh_structure) const override;

    };

}
