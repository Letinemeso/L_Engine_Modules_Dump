#pragma once

#include <Tools/Voxel/Voxel_2D/Voxel_2D.h>


namespace LGL
{

    bool voxel_2d_inside_circle(const Voxel_2D* _voxel, const float* _circle_center, float _circle_radius);
    bool voxel_2d_touches_circle(const Voxel_2D* _voxel, const float* _circle_center, float _circle_radius);

    bool voxel_2d_inside_triangle(const Voxel_2D* _voxel, const float* _vertex_0, const float* _vertex_1, const float* _vertex_2);
    bool voxel_2d_touches_triangle(const Voxel_2D* _voxel, const float* _vertex_0, const float* _vertex_1, const float* _vertex_2);

    bool voxel_2d_inside_rectangle(const Voxel_2D* _voxel, float _left, float _bottom, float _right, float _top);
    bool voxel_2d_touches_rectangle(const Voxel_2D* _voxel, float _left, float _bottom, float _right, float _top);

}
