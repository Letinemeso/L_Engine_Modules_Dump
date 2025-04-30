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
    float t2 = _t * _t;
    float t3 = t2 * _t;

    float base_weight = (1.0f - m_tension) * 0.5f;
    glm::vec3 term1 = base_weight * (-_0 + 3.0f * _1 - 3.0f * _2 + _3) * t3;
    glm::vec3 term2 = base_weight * (2.0f * _0 - 5.0f * _1 + 4.0f * _2 - _3) * t2;
    glm::vec3 term3 = base_weight * (-_0 + _2) * _t;
    glm::vec3 term4 = _1;

    return term1 + term2 + term3 + term4;
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

        glm::vec3 current_segment = p2 - p1;

        float segment_length = LEti::Math::vector_length(current_segment);
        unsigned int points_amount = (unsigned int)(segment_length / m_interpolation_step_length);
        if(points_amount == 0)
            points_amount = 1;

        Points_List interpolated_curve;
        for (unsigned int j = 0; j < points_amount; ++j)
        {
            float t = (float)j / (float)points_amount;
            glm::vec3 point = M_calculate_point(p0, p1, p2, p3, t);
            interpolated_curve.push_back(point);
        }

        glm::vec3 missing_length = p2 - *interpolated_curve.end();
        float step_ratio_multiplier = 1.0f / (float)interpolated_curve.size();

        Points_List::Iterator curve_point_it = interpolated_curve.begin();
        for(unsigned int c = 0; c < interpolated_curve.size(); ++c)
        {
            glm::vec3 additional_length = missing_length * (float)c * step_ratio_multiplier;
            *curve_point_it += additional_length;
            ++curve_point_it;
        }

        result.append((Points_List&&)interpolated_curve);

        ++point_it;
    }

    M_remove_edge_control_points();

    result.push_back(*m_points.end());

    return result;
}
