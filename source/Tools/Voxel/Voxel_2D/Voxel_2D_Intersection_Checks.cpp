#include <Tools/Voxel/Voxel_2D/Voxel_2D_Intersection_Checks.h>

using namespace LMD;


bool LMD::voxel_2d_inside_circle(const Voxel_2D* _voxel, const float* _circle_center, float _circle_radius)
{
    float radius_squared = _circle_radius * _circle_radius;

    float left_squared = _voxel->position_x() - _circle_center[0];
    left_squared *= left_squared;
    float right_squared = _voxel->position_x() + _voxel->size_x() - _circle_center[0];
    right_squared *= right_squared;
    float bottom_squared = _voxel->position_y() - _circle_center[1];
    bottom_squared *= bottom_squared;
    float top_squared = _voxel->position_y() + _voxel->size_y() - _circle_center[1];
    top_squared *= top_squared;

    if(left_squared + bottom_squared > radius_squared)
        return false;
    if(right_squared + bottom_squared > radius_squared)
        return false;
    if(left_squared + top_squared > radius_squared)
        return false;
    if(right_squared + top_squared > radius_squared)
        return false;

    return true;
}

bool LMD::voxel_2d_touches_circle(const Voxel_2D* _voxel, const float* _circle_center, float _circle_radius)
{
    float rectX = _voxel->position_x();
    float rectY = _voxel->position_y();
    float rectWidth = _voxel->size_x();
    float rectHeight = _voxel->size_y();

    float circleX = _circle_center[0];
    float circleY = _circle_center[1];

    float closestX = std::max(rectX, std::min(circleX, rectX + rectWidth));
    float closestY = std::max(rectY, std::min(circleY, rectY + rectHeight));

    float distanceX = circleX - closestX;
    float distanceY = circleY - closestY;
    float distanceSquared = distanceX * distanceX + distanceY * distanceY;

    return distanceSquared <= _circle_radius * _circle_radius;
}


bool point_inside_triangle(const float* p, const float* a, const float* b, const float* c)
{
    auto sign = [](const float p1[2], const float p2[2], const float p3[2])
    {
        return (p1[0] - p3[0]) * (p2[1] - p3[1]) - (p2[0] - p3[0]) * (p1[1] - p3[1]);
    };

    float d1 = sign(p, a, b);
    float d2 = sign(p, b, c);
    float d3 = sign(p, c, a);

    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

bool LMD::voxel_2d_inside_triangle(const Voxel_2D* _voxel, const float* _vertex_0, const float* _vertex_1, const float* _vertex_2)
{
    float rectCorners[4][2] = {
        {_voxel->position_x(),                     _voxel->position_y()},
        {_voxel->position_x() + _voxel->size_x(),  _voxel->position_y()},
        {_voxel->position_x() + _voxel->size_x(),  _voxel->position_y() + _voxel->size_y()},
        {_voxel->position_x(),                     _voxel->position_y() + _voxel->size_y()}
    };

    for (const auto& corner : rectCorners)
    {
        if (!point_inside_triangle(corner, _vertex_0, _vertex_1, _vertex_2))
            return false;
    }

    return true;
}

int orientation(const float* p, const float* q, const float* r)
{
    float val = (q[1] - p[1]) * (r[0] - q[0]) - (q[0] - p[0]) * (r[1] - q[1]);
    if (val == 0)
        return 0;
    return (val > 0) ? 1 : 2;
}

bool segments_intersect(const float* p1, const float* q1, const float* p2, const float* q2)
{
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4)
        return true;

    return false;
}

bool LMD::voxel_2d_touches_triangle(const Voxel_2D* _voxel, const float* _vertex_0, const float* _vertex_1, const float* _vertex_2)
{
    float rectCorners[4][2] = {
        {_voxel->position_x(),                     _voxel->position_y()},
        {_voxel->position_x() + _voxel->size_x(),  _voxel->position_y()},
        {_voxel->position_x() + _voxel->size_x(),  _voxel->position_y() + _voxel->size_y()},
        {_voxel->position_x(),                     _voxel->position_y() + _voxel->size_y()}
    };

    const float* triangleEdges[3][2] = {{_vertex_0, _vertex_1}, {_vertex_1, _vertex_2}, {_vertex_2, _vertex_0}};
    for (const auto& edge : triangleEdges)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (segments_intersect(edge[0], edge[1], rectCorners[i], rectCorners[(i + 1) % 4]))
                return true;
        }
    }

    return false;
}


bool LMD::voxel_2d_inside_rectangle(const Voxel_2D* _voxel, float _left, float _bottom, float _right, float _top)
{
    float voxel_left = _voxel->position_x();
    float voxel_right = voxel_left + _voxel->size_x();

    if(!(voxel_left >= _left && voxel_left <= _right && voxel_right >= _left && voxel_right <= _right))
        return false;

    float voxel_bottom = _voxel->position_y();
    float voxel_top = voxel_bottom + _voxel->size_y();

    if(!(voxel_bottom >= _bottom && voxel_bottom <= _top && voxel_top >= _bottom && voxel_top <= _top))
        return false;

    return true;
}

bool LMD::voxel_2d_touches_rectangle(const Voxel_2D* _voxel, float _left, float _bottom, float _right, float _top)
{
    float voxel_left = _voxel->position_x();
    float voxel_right = voxel_left + _voxel->size_x();
    float voxel_bottom = _voxel->position_y();
    float voxel_top = voxel_bottom + _voxel->size_y();

    bool touches_horizontally = false;
    if(voxel_left >= _left && voxel_left <= _right)
        touches_horizontally = true;
    else if(voxel_right >= _left && voxel_right <= _right)
        touches_horizontally = true;
    else if(_left >= voxel_left && _left <= voxel_right)
        touches_horizontally = true;
    else if(_right >= voxel_left && _right <= voxel_right)
        touches_horizontally = true;

    bool touches_vertically = false;
    if(voxel_bottom >= _bottom && voxel_bottom <= _top)
        touches_vertically = true;
    else if(voxel_top >= _bottom && voxel_top <= _top)
        touches_vertically = true;
    else if(_bottom >= voxel_bottom && _bottom <= voxel_top)
        touches_vertically = true;
    else if(_top >= voxel_bottom && _top <= voxel_top)
        touches_vertically = true;

    return touches_vertically && touches_horizontally;
}
