#include <Physics/Rigid_Body/Rigid_Body_Utility.h>

using namespace LMD;


glm::vec3 Rigid_Body_Utility::calculate_radius_vector(const Physics_Module__Rigid_Body& _rb, const glm::vec3& _point)
{
    return _point - _rb.center_of_mass();
}

glm::vec3 Rigid_Body_Utility::calculate_point_velocity(const Physics_Module__Rigid_Body& _rb, const glm::vec3& _radius_vector)
{
    glm::vec3 angular_velocity_component = LST::Math::cross_product(_rb.angular_velocity(), _radius_vector);
    glm::vec3 relative_velocity = _rb.velocity() + angular_velocity_component;
    return relative_velocity;
}
