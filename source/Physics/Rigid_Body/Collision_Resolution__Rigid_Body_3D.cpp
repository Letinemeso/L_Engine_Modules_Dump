#include <Physics/Rigid_Body/Collision_Resolution__Rigid_Body_3D.h>

#include <Stuff/Math_Stuff.h>

using namespace LMD;


bool Collision_Resolution__Rigid_Body_3D::M_resolve_dynamic_vs_dynamic(const LPhys::Intersection_Data& _id, float _dt)
{
    Physics_Module__Rigid_Body* pm1 = LV::cast_variable<Physics_Module__Rigid_Body>(_id.first);
    Physics_Module__Rigid_Body* pm2 = LV::cast_variable<Physics_Module__Rigid_Body>(_id.second);

    if(!pm1 || !pm2)
        return false;

    if(!m_default_collision_resolution.resolve(_id, _dt))
        return false;

    glm::vec3 vec_to_point_1 = _id.point - pm1->center_of_mass();
    glm::vec3 vec_to_point_2 = _id.point - pm2->center_of_mass();

    glm::vec3 point_velocity_1 = pm1->velocity() + LST::Math::cross_product(pm1->angular_velocity(), vec_to_point_1);
    glm::vec3 point_velocity_2 = pm2->velocity() + LST::Math::cross_product(pm2->angular_velocity(), vec_to_point_2);

    glm::vec3 relative_velocity = point_velocity_1 - point_velocity_2;
    float relative_velocity_along_normal = LST::Math::dot_product(relative_velocity, _id.normal);
    if(relative_velocity_along_normal >= 0.0f)
        return true;

    glm::vec3 perpendicular_1 = LST::Math::cross_product(vec_to_point_1, _id.normal);
    glm::vec3 perpendicular_2 = LST::Math::cross_product(vec_to_point_2, _id.normal);

    float mass_denom = (1.0f / pm1->mass() + 1.0f / pm2->mass() +
                        LST::Math::dot_product(perpendicular_1, pm1->moment_of_inertia_inverse() * perpendicular_1) +
                        LST::Math::dot_product(perpendicular_2, pm2->moment_of_inertia_inverse()  * perpendicular_2));
    // float effective_mass = 1.0f / mass_denom;

    float impulse_magnitude_normal = -1.0f * relative_velocity_along_normal;
    impulse_magnitude_normal = impulse_magnitude_normal / mass_denom;

    glm::vec3 total_impulse_vector = _id.normal * impulse_magnitude_normal;

    glm::vec3 new_linear_velocity_1 = pm1->velocity() - total_impulse_vector / pm1->mass();
    glm::vec3 new_linear_velocity_2 = pm2->velocity() + total_impulse_vector / pm2->mass();

    glm::vec3 angular_impulse_1 = LST::Math::cross_product(vec_to_point_1, -total_impulse_vector);
    glm::vec3 new_angular_velocity_1 = pm1->angular_velocity() + pm1->moment_of_inertia_inverse() * angular_impulse_1;
    glm::vec3 angular_impulse_2 = LST::Math::cross_product(vec_to_point_2, total_impulse_vector);
    glm::vec3 new_angular_velocity_2 = pm2->angular_velocity() + pm2->moment_of_inertia_inverse() * angular_impulse_2;

    pm1->set_velocity(new_linear_velocity_1);
    pm2->set_velocity(new_linear_velocity_2);

    pm1->set_angular_velocity(new_angular_velocity_1);
    pm2->set_angular_velocity(new_angular_velocity_2);

    return true;
}

bool Collision_Resolution__Rigid_Body_3D::M_resolve_dynamic_vs_static(const LPhys::Intersection_Data& _id, float _dt)
{
    LPhys::Physics_Module* dynamic_pm = _id.first->is_static() ? _id.second : _id.first;
    L_ASSERT(!dynamic_pm->is_static());

    Physics_Module__Rigid_Body* rb = LV::cast_variable<Physics_Module__Rigid_Body>(dynamic_pm);
    if(!rb)
        return false;

    m_default_collision_resolution.resolve(_id, _dt);

    glm::vec3 normal = _id.normal;
    if(dynamic_pm == _id.second)
        normal *= -1.0f;

    glm::vec3 radius_vector = _id.point - rb->center_of_mass();

    glm::vec3 contact_point_velocity = rb->velocity() + LST::Math::cross_product(rb->angular_velocity(), radius_vector);

    float relative_normal_velocity = LST::Math::dot_product(contact_point_velocity, normal);

    if (relative_normal_velocity >= 0.0f)
        return false;

    glm::vec3 radius_cross_normal = LST::Math::cross_product(radius_vector, normal);
    glm::vec3 inv_inertia_times_radius_cross_normal = rb->inertia_tensor_inverse() * radius_cross_normal;

    float effective_mass_denominator = (1.0f / rb->mass())
                                       + LST::Math::dot_product( normal, LST::Math::cross_product(inv_inertia_times_radius_cross_normal, radius_vector) );

    if(effective_mass_denominator < LST::Math::Float_Precision_Tolerance)
        return false;

    if(relative_normal_velocity < -1.5f)
        M_resolve_impulse(rb, normal, radius_vector, relative_normal_velocity, effective_mass_denominator);
    else
        M_resolve_contact(rb, normal, radius_vector, relative_normal_velocity, effective_mass_denominator);

    return true;
}


void Collision_Resolution__Rigid_Body_3D::M_resolve_impulse(Physics_Module__Rigid_Body* _rb, const glm::vec3& _normal, const glm::vec3& _radius_vector, float _normal_velocity, float _mass_denominator)
{
    float impulse_magnitude = -(1.0f + _rb->restitution()) * _normal_velocity / _mass_denominator;

    glm::vec3 velocity = _rb->velocity();
    velocity += (impulse_magnitude / _rb->mass()) * _normal;

    glm::vec3 angular_impulse = LST::Math::cross_product(_radius_vector, impulse_magnitude * _normal);
    glm::vec3 angular_velocity = _rb->angular_velocity();
    angular_velocity += _rb->inertia_tensor_inverse() * angular_impulse;

    float impulse_multiplier = _rb->restitution();

    velocity *= impulse_multiplier;
    angular_velocity *= impulse_multiplier;

    M_apply_friction(_rb, _normal, _radius_vector, _normal_velocity, velocity, angular_velocity);
    M_damp_velocities(velocity, angular_velocity);

    _rb->set_velocity( velocity );
    _rb->set_angular_velocity( angular_velocity );
}

void Collision_Resolution__Rigid_Body_3D::M_resolve_contact(Physics_Module__Rigid_Body* _rb, const glm::vec3& _normal, const glm::vec3& _radius_vector, float _normal_velocity, float _mass_denominator)
{
    float normal_impulse_magnitude = -_normal_velocity / _mass_denominator;

    glm::vec3 normal_impulse = normal_impulse_magnitude * _normal;
    glm::vec3 angular_impulse = LST::Math::cross_product(_radius_vector, normal_impulse);

    glm::vec3 velocity = _rb->velocity();
    glm::vec3 angular_velocity = _rb->angular_velocity();

    velocity += normal_impulse * _rb->mass_inverse();
    angular_velocity += _rb->inertia_tensor_inverse() * angular_impulse;

    M_apply_friction(_rb, _normal, _radius_vector, _normal_velocity, velocity, angular_velocity);
    M_damp_velocities(velocity, angular_velocity);

    _rb->set_velocity( velocity );
    _rb->set_angular_velocity( angular_velocity );
}


void Collision_Resolution__Rigid_Body_3D::M_apply_friction(Physics_Module__Rigid_Body* _rb, const glm::vec3& _normal, const glm::vec3& _radius_vector, float _normal_velocity, glm::vec3& _new_velocity, glm::vec3& _new_angular_velocity)
{
    glm::vec3 contact_velocity = _new_velocity + LST::Math::cross_product(_new_angular_velocity, _radius_vector);

    glm::vec3 tangent_velocity = contact_velocity - LST::Math::dot_product(contact_velocity, _normal) * _normal;

    float tangent_speed = LST::Math::vector_length(tangent_velocity);
    if (tangent_speed < LST::Math::Float_Precision_Tolerance)
        return;

    glm::vec3 tangent_direction = tangent_velocity / tangent_speed;

    glm::vec3 r_cross_t = LST::Math::cross_product(_radius_vector, tangent_direction);
    glm::vec3 angular_term = LST::Math::cross_product(_rb->inertia_tensor_inverse() * r_cross_t, _radius_vector);

    float effective_mass = _rb->mass_inverse() + LST::Math::dot_product(tangent_direction, angular_term);

    if (effective_mass <= 0.0f)
        return;

    float desired_impulse = -tangent_speed / effective_mass;

    float max_friction_impulse = _rb->traction() * _normal_velocity;

    float friction_impulse_magnitude = glm::clamp( desired_impulse, -max_friction_impulse, max_friction_impulse );

    glm::vec3 friction_impulse = friction_impulse_magnitude * tangent_direction;

    _new_velocity += friction_impulse * _rb->mass_inverse();
    _new_angular_velocity += _rb->inertia_tensor_inverse() * LST::Math::cross_product(_radius_vector, friction_impulse);
}


void Collision_Resolution__Rigid_Body_3D::M_damp_velocities(glm::vec3& _velocity, glm::vec3& _angular_velocity) const
{
    float raw_velocity_squared = LST::Math::vector_length_squared(_velocity);
    float raw_angular_velocity_squared = LST::Math::vector_length_squared(_angular_velocity);

    if(raw_velocity_squared < m_hard_damping_velocity_threshold_squared)
        _velocity *= 0.0f;
    else if(raw_velocity_squared < m_soft_damping_min_velocity_squared)
        _velocity *= m_soft_damping_multiplier;

    if(raw_angular_velocity_squared < m_hard_damping_angular_velocity_threshold_squared)
        _angular_velocity *= 0.0f;
    else if(raw_angular_velocity_squared < m_soft_damping_min_angular_velocity_squared)
        _angular_velocity *= m_soft_damping_multiplier;
}



bool Collision_Resolution__Rigid_Body_3D::resolve(const LPhys::Intersection_Data &_id, float _dt)
{
    if(!_id.first->is_static() && !_id.second->is_static())
        return M_resolve_dynamic_vs_dynamic(_id, _dt);

    if(_id.first->is_static() && _id.second->is_static())
        return false;

    return M_resolve_dynamic_vs_static(_id, _dt);
}
