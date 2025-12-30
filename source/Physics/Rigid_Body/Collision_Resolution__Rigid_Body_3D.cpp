#include <Physics/Rigid_Body/Collision_Resolution__Rigid_Body_3D.h>

#include <Math_Stuff.h>

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

    glm::vec3 point_velocity_1 = pm1->velocity() + LEti::Math::cross_product(pm1->angular_velocity(), vec_to_point_1);
    glm::vec3 point_velocity_2 = pm2->velocity() + LEti::Math::cross_product(pm2->angular_velocity(), vec_to_point_2);

    glm::vec3 relative_velocity = point_velocity_1 - point_velocity_2;
    float relative_velocity_along_normal = LEti::Math::dot_product(relative_velocity, _id.normal);
    if(relative_velocity_along_normal >= 0.0f)
        return true;

    glm::vec3 perpendicular_1 = LEti::Math::cross_product(vec_to_point_1, _id.normal);
    glm::vec3 perpendicular_2 = LEti::Math::cross_product(vec_to_point_2, _id.normal);

    float mass_denom = (1.0f / pm1->mass() + 1.0f / pm2->mass() +
                        LEti::Math::dot_product(perpendicular_1, pm1->moment_of_inertia_inverse() * perpendicular_1) +
                        LEti::Math::dot_product(perpendicular_2, pm2->moment_of_inertia_inverse()  * perpendicular_2));
    // float effective_mass = 1.0f / mass_denom;

    float impulse_magnitude_normal = -1.0f * relative_velocity_along_normal;
    impulse_magnitude_normal = impulse_magnitude_normal / mass_denom;

    glm::vec3 total_impulse_vector = _id.normal * impulse_magnitude_normal;

    glm::vec3 new_linear_velocity_1 = pm1->velocity() - total_impulse_vector / pm1->mass();
    glm::vec3 new_linear_velocity_2 = pm2->velocity() + total_impulse_vector / pm2->mass();

    glm::vec3 angular_impulse_1 = LEti::Math::cross_product(vec_to_point_1, -total_impulse_vector);
    glm::vec3 new_angular_velocity_1 = pm1->angular_velocity() + pm1->moment_of_inertia_inverse() * angular_impulse_1;
    glm::vec3 angular_impulse_2 = LEti::Math::cross_product(vec_to_point_2, total_impulse_vector);
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

    glm::vec3 contact_point_velocity = rb->velocity() + LEti::Math::cross_product(rb->angular_velocity(), radius_vector);

    float relative_normal_velocity = LEti::Math::dot_product(contact_point_velocity, normal);

    if (relative_normal_velocity >= 0.0f)
        return false;

    glm::vec3 radius_cross_normal = LEti::Math::cross_product(radius_vector, normal);
    glm::vec3 inv_inertia_times_radius_cross_normal = rb->inertia_tensor_inverse() * radius_cross_normal;

    float effective_mass_denominator = (1.0f / rb->mass())
                                       + LEti::Math::dot_product( normal, LEti::Math::cross_product(inv_inertia_times_radius_cross_normal, radius_vector) );

    float impulse_magnitude = -(1.0f + rb->restitution()) * relative_normal_velocity / effective_mass_denominator;

    glm::vec3 velocity = rb->velocity();
    velocity += (impulse_magnitude / rb->mass()) * normal;

    glm::vec3 angular_impulse = LEti::Math::cross_product(radius_vector, impulse_magnitude * normal);
    glm::vec3 angular_velocity = rb->angular_velocity();
    angular_velocity += rb->inertia_tensor_inverse() * angular_impulse;

    float impulse_multiplier = rb->restitution();

    velocity *= impulse_multiplier;
    angular_velocity *= impulse_multiplier;

    M_damp_velocities(velocity, angular_velocity);

    rb->set_velocity( velocity );
    rb->set_angular_velocity( angular_velocity );

    return true;
}


void Collision_Resolution__Rigid_Body_3D::M_damp_velocities(glm::vec3& _velocity, glm::vec3& _angular_velocity) const
{
    float raw_velocity_squared = LEti::Math::vector_length_squared(_velocity);
    float raw_angular_velocity_squared = LEti::Math::vector_length_squared(_angular_velocity);

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
