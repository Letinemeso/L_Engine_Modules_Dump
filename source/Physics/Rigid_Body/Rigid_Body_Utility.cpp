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

void Rigid_Body_Utility::apply_force_in_point(Physics_Module__Rigid_Body& _rb, const glm::vec3& _point, const glm::vec3& _force)
{
    glm::vec3 radius_vector = Rigid_Body_Utility::calculate_radius_vector(_rb, _point);
    glm::vec3 moment = LST::Math::cross_product(radius_vector, _force);

    glm::vec3 velocity_increment = _force / _rb.mass();
    glm::vec3 angular_velocity_increment = _rb.inertia_tensor_inverse() * moment;

    _rb.apply_linear_impulse(velocity_increment);
    _rb.apply_rotation(angular_velocity_increment);
}

void Rigid_Body_Utility::apply_force_in_point(Physics_Module__Rigid_Body& _rb, const glm::vec3& _point, const glm::vec3& _direction, float _force)
{
    Rigid_Body_Utility::apply_force_in_point(_rb, _point, _direction * _force);
}
