#pragma once

#include <vec2.hpp>

#include <L_Debug/L_Debug.h>
#include <Data_Structures/Vector.h>


namespace LMD
{

    class Triangle
    {
    private:
        glm::vec2 m_points[3];

    public:
        inline glm::vec2& operator[](unsigned int _index) { L_ASSERT(_index < 3); return m_points[_index]; }
        inline const glm::vec2& operator[](unsigned int _index) const { L_ASSERT(_index < 3); return m_points[_index]; }

    };

    using Triangles_Vec = LDS::Vector<Triangle>;
    using Points_Vec = LDS::Vector<glm::vec2>;

    Triangles_Vec triangulate(const Points_Vec& _points);

}
