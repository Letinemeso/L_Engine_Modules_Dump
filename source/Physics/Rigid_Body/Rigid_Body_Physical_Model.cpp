#include <Physics/Rigid_Body/Rigid_Body_Physical_Model.h>

#include <Math_Stuff.h>

using namespace LMD;



Rigid_Body_Physical_Model::Rigid_Body_Physical_Model()
    : LPhys::Physical_Model()
{

}

Rigid_Body_Physical_Model::Rigid_Body_Physical_Model(const Rigid_Body_Physical_Model &_other)
    : LPhys::Physical_Model(_other)
{
    set_mass(_other.m_mass);
}

Rigid_Body_Physical_Model::~Rigid_Body_Physical_Model()
{

}



float Rigid_Body_Physical_Model::M_calculate_polygon_area(const LPhys::Polygon& _polygon) const
{
    glm::vec3 edge_0 = _polygon[1] - _polygon[0];
    glm::vec3 edge_1 = _polygon[2] - _polygon[0];

    glm::vec3 cross = LEti::Math::cross_product(edge_0, edge_1);

    return LEti::Math::vector_length(cross) * 0.5f;
}

LDS::Vector<float> Rigid_Body_Physical_Model::M_calculate_polygons_areas() const
{
    LDS::Vector<float> result(get_polygons()->amount());

    for(unsigned int i = 0; i < get_polygons()->amount(); ++i)
    {
        const LPhys::Polygon& polygon = *get_polygons()->get_polygon(i);
        result.push( M_calculate_polygon_area(polygon) );
    }

    return result;
}

float Rigid_Body_Physical_Model::M_calculate_total_area(const LDS::Vector<float>& _polygons_areas) const
{
    float result = 0.0f;
    for(unsigned int i = 0; i < get_polygons()->amount(); ++i)
        result += _polygons_areas[i];
    return result;
}


glm::vec3 Rigid_Body_Physical_Model::M_calculate_central_point() const
{
    glm::vec3 result(0.0f, 0.0f, 0.0f);

    for(unsigned int i = 0; i < get_polygons()->amount(); ++i)
    {
        const LPhys::Polygon& polygon = *get_polygons()->get_polygon(i);
        result += polygon.center();
    }

    result /= (float)get_polygons()->amount();

    return result;
}

glm::vec3 Rigid_Body_Physical_Model::M_calculate_center_of_mass() const
{
    LDS::Vector<float> polygons_areas = M_calculate_polygons_areas();
    float total_area = M_calculate_total_area(polygons_areas);

    glm::vec3 center_point = M_calculate_central_point();

    glm::vec3 total_center_of_mass_stride = {0.0f, 0.0f, 0.0f};

    for(unsigned int i = 0; i < get_polygons()->amount(); ++i)
    {
        const LPhys::Polygon& polygon = *get_polygons()->get_polygon(i);

        float weight_ratio = polygons_areas[i] / total_area;

        glm::vec3 vec_to_polygon = polygon.center() - center_point;
        vec_to_polygon *= weight_ratio;

        total_center_of_mass_stride += vec_to_polygon;
    }

    return center_point + total_center_of_mass_stride;
}

float Rigid_Body_Physical_Model::M_calculate_moment_of_inertia() const
{
    constexpr float Point_Mass_Multiplier = 1.0f / 3.0f;

    LDS::Vector<float> polygons_areas = M_calculate_polygons_areas();
    float total_area = M_calculate_total_area(polygons_areas);

    float result = 0.0f;

    for(unsigned int p_i = 0; p_i < get_polygons()->amount(); ++p_i)
    {
        const LPhys::Polygon& polygon = *get_polygons()->get_polygon(p_i);

        float polygon_mass = (polygons_areas[p_i] / total_area) * m_mass;
        float point_mass = polygon_mass * Point_Mass_Multiplier;

        for(unsigned int i = 0; i < 3; ++i)
        {
            if(!polygon.segment_can_collide(i))
                continue;

            float distance_squared = LEti::Math::vector_length_squared(polygon[i] - center_of_mass());
            result += distance_squared * point_mass;
        }
    }

    return result;
}

glm::mat3x3 Rigid_Body_Physical_Model::M_calculate_inertia_tensor() const
{
    constexpr float Point_Mass_Multiplier = 1.0f / 3.0f;

    LDS::Vector<float> polygons_areas = M_calculate_polygons_areas();
    float total_area = M_calculate_total_area(polygons_areas);

    glm::mat3x3 result = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    };

    for(unsigned int p_i = 0; p_i < get_polygons()->amount(); ++p_i)
    {
        const LPhys::Polygon& polygon = *get_polygons()->get_polygon(p_i);

        float polygon_mass = (polygons_areas[p_i] / total_area) * m_mass;
        float point_mass = polygon_mass * Point_Mass_Multiplier;

        for(unsigned int i = 0; i < 3; ++i)
        {
            if(!polygon.segment_can_collide(i))
                continue;

            glm::vec3 point_wo_offset = polygon[i] - center_of_mass();
            glm::vec3 point_squared = point_wo_offset * point_wo_offset;

            result[0][0] += point_mass * (point_squared.y * point_squared.z);
            result[1][1] += point_mass * (point_squared.x * point_squared.z);
            result[2][2] += point_mass * (point_squared.x * point_squared.y);

            result[0][1] -= point_mass * point_wo_offset.x * point_wo_offset.y;
            result[0][2] -= point_mass * point_wo_offset.x * point_wo_offset.z;
            result[1][2] -= point_mass * point_wo_offset.y * point_wo_offset.z;
        }
    }

    result[1][0] = result[0][1];
    result[2][0] = result[0][2];
    result[2][1] = result[1][2];

    return result;
}



void Rigid_Body_Physical_Model::update(const glm::mat4x4 &_matrix)
{
    Physical_Model::update(_matrix);

    m_center_of_mass = M_calculate_center_of_mass();
    m_moment_of_inertia = M_calculate_moment_of_inertia();
    m_inertia_tensor = M_calculate_inertia_tensor();
    m_inertia_tensor_inverse = glm::inverse((m_inertia_tensor));
}


void Rigid_Body_Physical_Model::set_mass(float _mass)
{
    m_mass = _mass;


}
