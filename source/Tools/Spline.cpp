#include <Tools/Spline.h>

#include <Math_Stuff.h>

using namespace LMD;


Spline::Spline()
{

}

Spline::~Spline()
{

}



void Spline::M_add_edge_control_points() const
{
    auto first_it = m_points.begin();
    glm::vec3 first_point = *first_it;
    ++first_it;
    glm::vec3 first_segment = *first_it - first_point;
    glm::vec3 first_control_point = first_point - first_segment;

    auto last_it = m_points.end();
    glm::vec3 last_point = *last_it;
    --last_it;
    glm::vec3 last_segment = last_point - *last_it;
    glm::vec3 last_control_point = last_point + last_segment;

    m_points.push_front(first_control_point);
    m_points.push_back(last_control_point);
}

void Spline::M_remove_edge_control_points() const
{
    m_points.pop_back();
    m_points.pop_front();
}


glm::vec3 Spline::M_calculate_point(const glm::vec3& _0, const glm::vec3& _1, const glm::vec3& _2, const glm::vec3& _3, float _t) const
{
    constexpr float normalizer = 0.5f;

    return normalizer * (
               (2.0f * _1) +
               (-_0 + _2) * _t +
               (2.0f * _0 - 5.0f * _1 + 4.0f * _2 - _3) * _t * _t +
               (-_0 + 3.0f * _1 - 3.0f * _2 + _3) * _t * _t * _t
               );
}



Spline::Points_List Spline::interpolate() const
{
    L_ASSERT(m_points.size() >= 3);
    L_ASSERT(m_interpolation_step_length > 0.0f);

    M_add_edge_control_points();

    Points_List result;

    Points_List::Iterator point_it = m_points.begin();
    for (unsigned int i = 0; i < m_points.size() - 3; ++i)
    {
        Points_List::Iterator temp_point_it = point_it;
        const glm::vec3& p0 = *temp_point_it;
        ++temp_point_it;
        const glm::vec3& p1 = *temp_point_it;
        ++temp_point_it;
        const glm::vec3& p2 = *temp_point_it;
        ++temp_point_it;
        const glm::vec3& p3 = *temp_point_it;
        ++temp_point_it;

        float segment_length = LEti::Math::vector_length(p1 - p2);
        unsigned int points_amount = (unsigned int)(segment_length / m_interpolation_step_length);

        for (unsigned int j = 0; j < points_amount; ++j)
        {
            float t = (float)j / (float)points_amount;
            glm::vec3 point = M_calculate_point(p0, p1, p2, p3, t);
            result.push_back(point);
        }

        ++point_it;
    }

    M_remove_edge_control_points();

    return result;
}
