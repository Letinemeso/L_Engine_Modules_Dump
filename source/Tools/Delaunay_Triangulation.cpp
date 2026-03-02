#include <Tools/Delaunay_Triangulation.h>

#include <Stuff/Math_Stuff.h>
#include <Data_Structures/List.h>

#include <Collision_Detection/Primitives/Lines_Intersection.h>

using namespace LMD;


namespace LMD
{
    constexpr unsigned int Daddy_Index_0 = LST::Math::Max_Unsigned_Int - 2;
    constexpr unsigned int Daddy_Index_1 = LST::Math::Max_Unsigned_Int - 1;
    constexpr unsigned int Daddy_Index_2 = LST::Math::Max_Unsigned_Int;

    struct Index_And_Point
    {
        glm::vec2 point;
        unsigned int index;
    };

    class Index_Triangle
    {
    private:
        Index_And_Point m_data[3];

    public:
        inline Index_And_Point& operator[](unsigned int _index) { L_ASSERT(_index < 3); return m_data[_index]; }
        inline const Index_And_Point& operator[](unsigned int _index) const { L_ASSERT(_index < 3); return m_data[_index]; }

    };
    using Index_Triangles_List = LDS::List<Index_Triangle>;

    struct Neighbor_Triangle_Data
    {
        Index_Triangle* triangle = nullptr;
        unsigned int common_side_offset_index = 0;
    };

    struct Circle
    {
        glm::vec2 center;
        float radius_squared;
    };


    void debug_print_triangle(const Index_Triangle& _triangle)
    {
        for(unsigned int i = 0; i < 3; ++i)
            std::cout << "[" << i << "]: id: " << _triangle[i].index << "(x: " << _triangle[i].point.x << ", y: " << _triangle[i].point.y << ")\n";
        std::cout << std::endl;
    };

    void debug_print_list(const Index_Triangles_List& _list)
    {
        unsigned int counter = 0;
        for(Index_Triangles_List::Const_Iterator it = _list.begin(); !it.end_reached(); ++it)
        {
            const Index_Triangle& triangle = *it;

            std::cout << "Triangle #" << counter << ": " << std::endl;
            debug_print_triangle(triangle);

            ++counter;
        }
        std::cout << std::endl;
    }


    Index_Triangle calculate_daddy_triangle(const Points_Vec& _points)
    {
        glm::vec2 center = {0.0f, 0.0f};
        glm::vec2 min = {0.0f, 0.0f};
        glm::vec2 max = {0.0f, 0.0f};

        for(unsigned int i = 0; i < _points.size(); ++i)
        {
            const glm::vec2& point = _points[i];

            center += point;

            if(min.x > point.x)
                min.x = point.x;
            if(min.y > point.y)
                min.y = point.y;

            if(max.x < point.x)
                max.x = point.x;
            if(max.y < point.y)
                max.y = point.y;
        }

        center /= (float)_points.size();

        glm::vec2 size = max - min;
        glm::vec2 size_halved = size * 0.5f;

        constexpr float Big_Magic_Multiplier = 3.0f;
        float daddy_circle_radius = sqrtf( (size_halved.x * size_halved.x) + (size_halved.y * size_halved.y) ) * Big_Magic_Multiplier;

        Index_Triangle result;
        result[0] = { {center.x, center.y + (daddy_circle_radius * 2.0f)}, Daddy_Index_0 };
        result[1] = { {center.x - (LST::Math::SQRT_3 * daddy_circle_radius), center.y - daddy_circle_radius}, Daddy_Index_1 };
        result[2] = { {center.x + (LST::Math::SQRT_3 * daddy_circle_radius), center.y - daddy_circle_radius}, Daddy_Index_2 };

        return result;
    }

    bool point_inside_triangle(const Index_Triangle& _triangle, const glm::vec2& _point)
    {
        for(unsigned int i = 0; i < 3; ++i)
        {
            glm::vec2 segment_diff = _triangle[(i + 1) % 3].point - _triangle[i].point;
            glm::vec2 point_start_diff = _point - _triangle[i].point;

            float comparison_value = (segment_diff.x * point_start_diff.y) - (segment_diff.y * point_start_diff.x);
            if(comparison_value < 0.0f)
                return false;
        }

        return true;
    }

    Index_Triangles_List::Iterator find_owner_triangle(Index_Triangles_List& _list, const glm::vec2& _point)
    {
        for(Index_Triangles_List::Iterator it = _list.begin(); !it.end_reached(); ++it)
        {
            if(point_inside_triangle(*it, _point))
                return it;
        }

        return {};
    }

    Neighbor_Triangle_Data find_neighbor_triangle(Index_Triangles_List& _list, unsigned int _point_index_0, unsigned int _point_index_1)
    {
        for(Index_Triangles_List::Iterator it = _list.begin(); !it.end_reached(); ++it)
        {
            Index_Triangle& triangle = *it;

            for(unsigned int i = 0; i < 3; ++i)
            {
                if(triangle[i].index != _point_index_0)
                    continue;

                if(triangle[(i + 1) % 3].index == _point_index_1)
                    return { &triangle, i };
            }
        }

        return {};
    }

    Circle calculate_daddy_circle(const Index_Triangle& _triangle)
    {
        glm::vec2 segment_0 = _triangle[1].point - _triangle[0].point;
        glm::vec2 segment_1 = _triangle[2].point - _triangle[1].point;

        glm::vec2 segment_0_perpendicular = { -segment_0.y, segment_0.x };
        glm::vec2 segment_1_perpendicular = { -segment_1.y, segment_1.x };

        glm::vec2 segment_0_center = _triangle[0].point + (segment_0 * 0.5f);
        glm::vec2 segment_1_center = _triangle[1].point + (segment_1 * 0.5f);

        LPhys::Line line_0({segment_0_center, 0.0f}, {segment_0_center + segment_0_perpendicular, 0.0f});
        LPhys::Line line_1({segment_1_center, 0.0f}, {segment_1_center + segment_1_perpendicular, 0.0f});

        LPhys::Lines_Intersection_Data id = line_0.calculate_intersection_with(line_1);
        if(id.intersection != LPhys::Lines_Intersection_Data::Intersection)
            return {{0.0f, 0.0f}, -1.0f};

        Circle result;
        result.center = id.point;
        result.radius_squared = LST::Math::vector_length_squared(result.center - _triangle[0].point);

        return result;
    }

    void flip_side(Index_Triangles_List& _list, Index_Triangle& _triangle_0, Index_Triangle& _triangle_1, unsigned int _common_side_offset_index_0, unsigned int _common_side_offset_index_1);

    void flip_side_if_needed(Index_Triangles_List& _list, Index_Triangle& _triangle, unsigned int _common_side_offset_index)
    {
        unsigned int point_index_0 = _triangle[_common_side_offset_index].index;
        unsigned int point_index_1 = _triangle[(_common_side_offset_index + 1) % 3].index;

        Neighbor_Triangle_Data maybe_neighbor_triangle = find_neighbor_triangle(_list, point_index_1, point_index_0);
        if(!maybe_neighbor_triangle.triangle)
            return;

        Index_Triangle& neighbour_triangle = *maybe_neighbor_triangle.triangle;

        Circle daddy_circle = calculate_daddy_circle(_triangle);
        if(daddy_circle.radius_squared < 0.0f)
            return;

        unsigned int opposing_vertex_index = (maybe_neighbor_triangle.common_side_offset_index + 2) % 3;
        const glm::vec2& opposing_vertex = neighbour_triangle[opposing_vertex_index].point;

        float distance_to_center_squared = LST::Math::vector_length_squared(opposing_vertex - daddy_circle.center);
        if(distance_to_center_squared >= daddy_circle.radius_squared)
            return;

        flip_side(_list, _triangle, neighbour_triangle, _common_side_offset_index, maybe_neighbor_triangle.common_side_offset_index);
    }

    void flip_side(Index_Triangles_List& _list, Index_Triangle& _triangle_0, Index_Triangle& _triangle_1, unsigned int _common_side_offset_index_0, unsigned int _common_side_offset_index_1)
    {
        glm::vec2 vertices[4];
        unsigned int indices[4];

        /*      current
           0
          /|\
        /  |  \
      /    |    \
   3/      |      \2
    \      |      /
      \    |    /
        \  |  /
          \|/
           1
*/

        vertices[0] = _triangle_0[_common_side_offset_index_0].point;
        indices[0] = _triangle_0[_common_side_offset_index_0].index;

        vertices[1] = _triangle_0[(_common_side_offset_index_0 + 1) % 3].point;
        indices[1] = _triangle_0[(_common_side_offset_index_0 + 1) % 3].index;

        vertices[2] = _triangle_0[(_common_side_offset_index_0 + 2) % 3].point;
        indices[2] = _triangle_0[(_common_side_offset_index_0 + 2) % 3].index;

        vertices[3] = _triangle_1[(_common_side_offset_index_1 + 2) % 3].point;
        indices[3] = _triangle_1[(_common_side_offset_index_1 + 2) % 3].index;

        /*      target
           0
          / \
        /     \
      /         \
   3/____________\2
    \            /
      \         /
        \     /
          \ /
           1
*/

        _triangle_0[0] = {vertices[2], indices[2]};
        _triangle_0[1] = {vertices[3], indices[3]};
        _triangle_0[2] = {vertices[1], indices[1]};

        _triangle_1[0] = {vertices[2], indices[2]};
        _triangle_1[1] = {vertices[0], indices[0]};
        _triangle_1[2] = {vertices[3], indices[3]};

        flip_side_if_needed(_list, _triangle_0, 0);
        flip_side_if_needed(_list, _triangle_0, 1);
        flip_side_if_needed(_list, _triangle_1, 1);
        flip_side_if_needed(_list, _triangle_1, 2);
    }

    void split_triangle(Index_Triangles_List& _list, const Index_Triangle& _triangle, const glm::vec2& _point, unsigned int _point_index)
    {
        Index_Triangle* new_triangles[3];

        for(unsigned int i = 0; i < 3; ++i)
        {
            Index_Triangle new_triangle;
            new_triangle[0] = _triangle[i];
            new_triangle[1] = _triangle[(i + 1) % 3];
            new_triangle[2] = {_point, _point_index};

            _list.push_back(new_triangle);
            new_triangles[i] = &_list.back();
        }

        // std::cout << "Triangles before balancing " << _point_index << ":\n";
        // debug_print_list(_list);

        for(unsigned int i = 0; i < 3; ++i)
            flip_side_if_needed(_list, *new_triangles[i], 0);
    }

    bool is_daddy_triangle(const Index_Triangle& _triangle)
    {
        for(unsigned int i = 0; i < 3; ++i)
        {
            if(_triangle[i].index >= Daddy_Index_0)
                return true;
        }
        return false;
    }

    void purge_daddy_triangle(Index_Triangles_List& _list)
    {
        Index_Triangles_List::Iterator it = _list.begin();
        while(!it.end_reached())
        {
            const Index_Triangle& triangle = *it;
            if(is_daddy_triangle(triangle))
                it = _list.erase_and_iterate_forward(it);
            else
                ++it;
        }
    }

    Triangle triangle_from_index_triangle(const Index_Triangle& _index_triangle)
    {
        Triangle result;
        for(unsigned int i = 0; i < 3; ++i)
            result[i] = _index_triangle[i].point;
        return result;
    }
}



Triangles_Vec LMD::triangulate(const Points_Vec& _points)
{
    Index_Triangles_List list;
    list.push_back( calculate_daddy_triangle(_points) );

    // std::cout << "Initial daddy triangle: \n";
    // debug_print_list(list);

    for(unsigned int i = 0; i < _points.size(); ++i)
    {
        Index_Triangles_List::Iterator owner_triangle_it = find_owner_triangle(list, _points[i]);
        if(!owner_triangle_it.is_ok())
        {
            L_ASSERT(owner_triangle_it.is_ok());
        }
        L_ASSERT(owner_triangle_it.is_ok());

        Index_Triangle triangle = *owner_triangle_it;
        list.erase(owner_triangle_it);

        split_triangle(list, triangle, _points[i], i);

        // std::cout << "Added point #" << i << ":\n";
        // debug_print_list(list);
    }

    purge_daddy_triangle(list);

    Triangles_Vec result(list.size());
    for(Index_Triangles_List::Iterator it = list.begin(); !it.end_reached(); ++it)
        result.push( triangle_from_index_triangle(*it) );

    return result;
}
