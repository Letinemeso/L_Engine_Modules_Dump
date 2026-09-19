#include <Tools/Mesh_Generation/3D/Mesh_Smoothers/Mesh_3D_Smoother__Neighbor_Averaging.h>

using namespace LMD;


void Mesh_3D_Smoother__Neighbor_Averaging::M_append_points_neighbors_for_triangle(Mesh_3D_Utility::Point_Neighbors_Vec& _neighbors, const Mesh_3D_Utility::Triangle& _triangle) const
{
    for(unsigned int i = 0; i < 3; ++i)
    {
        Mesh_3D_Utility::IDs_Vec& ids_vec = _neighbors[ _triangle.id[i] ];

        unsigned int neighbor_0 = _triangle.id[(i + 1) % 3];
        unsigned int neighbor_1 = _triangle.id[(i + 2) % 3];

        if(!ids_vec.contains(neighbor_0))
            ids_vec.push(neighbor_0);
        if(!ids_vec.contains(neighbor_1))
            ids_vec.push(neighbor_1);
    }
}

Mesh_3D_Utility::Point_Neighbors_Vec Mesh_3D_Smoother__Neighbor_Averaging::M_update_point_neighbors(const Mesh_3D_Utility::Voxel_Triangles_Map& _mesh_structure, unsigned int _points_amount) const
{
    Mesh_3D_Utility::Point_Neighbors_Vec result(_points_amount);
    result.mark_full();

    for(Mesh_3D_Utility::Voxel_Triangles_Map::Const_Iterator it = _mesh_structure.iterator(); !it.end_reached(); ++it)
    {
        const Mesh_3D_Utility::Triangles_Vec& triangles = *it;

        for(unsigned int i = 0; i < triangles.size(); ++i)
            M_append_points_neighbors_for_triangle( result, triangles[i] );
    }

    return result;
}


glm::vec3 Mesh_3D_Smoother__Neighbor_Averaging::M_smooth_point(const glm::vec3& _point, const Mesh_3D_Utility::Points_Vec& _old_mesh, const Mesh_3D_Utility::IDs_Vec& _neighbors_ids) const
{
    L_ASSERT(_neighbors_ids.size() > 0);

    glm::vec3 stride = {0.0f, 0.0f, 0.0f};

    for(unsigned int i = 0; i < _neighbors_ids.size(); ++i)
    {
        const glm::vec3& neighbor_point = _old_mesh[ _neighbors_ids[i] ];
        stride += neighbor_point - _point;
    }

    stride /= (float)_neighbors_ids.size();

    return _point + stride;
}

Mesh_3D_Utility::Points_Vec Mesh_3D_Smoother__Neighbor_Averaging::M_smooth(const Mesh_3D_Utility::Points_Vec& _old_mesh, const Mesh_3D_Utility::Point_Neighbors_Vec& _points_neighbors) const
{
    Mesh_3D_Utility::Points_Vec smoothed_points(_old_mesh.size());
    smoothed_points.mark_full();

    for(unsigned int i = 0; i < _old_mesh.size(); ++i)
    {
        const glm::vec3& original_point = _old_mesh[i];
        const Mesh_3D_Utility::IDs_Vec& neighbors_ids = _points_neighbors[i];
        smoothed_points[i] = M_smooth_point(original_point, _old_mesh, neighbors_ids);
    }

    return smoothed_points;
}



Mesh_3D_Utility::Points_Vec Mesh_3D_Smoother__Neighbor_Averaging::smooth(const Mesh_3D_Utility::Points_Vec& _old_mesh,
                                                                         const Mesh_3D_Utility::Voxel_Triangles_Map& _mesh_structure) const
{
    Mesh_3D_Utility::Point_Neighbors_Vec neighbors = M_update_point_neighbors(_mesh_structure, _old_mesh.size());

    Mesh_3D_Utility::Points_Vec result = M_smooth(_old_mesh, neighbors);
    for(unsigned int i = 0; i < m_repetitions_amount - 1; ++i)
        result = M_smooth(result, neighbors);

    return result;
}
