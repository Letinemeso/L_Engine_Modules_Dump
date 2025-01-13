#pragma once

#include <vec3.hpp>

#include <Data_Structures/List.h>


namespace LMD
{

    class Spline
    {
    public:
        using Points_List = LDS::List<glm::vec3>;

    private:
        mutable Points_List m_points;   //  "interpolate()" will duplicate first and last point and then remove them. no actual data should be changed

    private:
        float m_interpolation_step_length = 0.0f;   //  defines distance between points in the list returned by "interpolate()"
        float m_tension = 0.0f;                     //  defines "curviness" of the spline. 0.0f -> as curvy as possible; 1.0f -> completely straight, unlike the user >:D

    public:
        Spline();
        ~Spline();

    public:
        inline void set_interpolation_step_length(float _value) { m_interpolation_step_length = _value; }
        inline void set_tension(float _value) { L_ASSERT(_value >= 0.0f && _value <= 1.0f); m_tension = _value; }
        inline void add_point(const glm::vec3& _point) { m_points.push_back(_point); }
        inline void set_points(const Points_List& _points) { m_points = _points; }
        inline void set_points(Points_List&& _points) { m_points = (Points_List&&)_points; }
        inline void clear_points() { m_points.clear(); }

    private:
        void M_add_edge_control_points() const;
        void M_remove_edge_control_points() const;

        glm::vec3 M_calculate_point(const glm::vec3& _0, const glm::vec3& _1, const glm::vec3& _2, const glm::vec3& _3, float _t) const;

    public:
        Points_List interpolate() const;    //  some "catmull rom" stuff here

    };

}
