#include <Physics/Rigid_Body/Collision_Resolution__Rigid_Body_3D.h>

#include <Math_Stuff.h>

using namespace LMD;


float Collision_Resolution__Rigid_Body_3D::M_calculate_kinetic_energy(const glm::vec3& _velocity, const glm::vec3& _angular_velocity, float _mass, float _moment_of_inertia) const
{
    float velocity = LEti::Math::vector_length(_velocity);

    float angular_velocity_squared = LEti::Math::vector_length_squared(_angular_velocity);

    float movemental = (_mass * velocity * velocity) * 0.5f;
    float rotational = (_moment_of_inertia * angular_velocity_squared) / 2.0f;

    return movemental + rotational;
}


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

    glm::vec3 normal = _id.normal;
    if(dynamic_pm == _id.second)
        normal *= -1.0f;

    float ke_before = M_calculate_kinetic_energy(rb->velocity(), rb->angular_velocity(), rb->mass(), rb->moment_of_inertia());

    const glm::vec3& center_of_mass = rb->center_of_mass();
    const glm::vec3 vec_to_point = _id.point - center_of_mass;

    const glm::vec3 velocity_at_contact = rb->velocity() +
                                          LEti::Math::cross_product(rb->angular_velocity(), vec_to_point);

    const float relative_velocity_normal = LEti::Math::dot_product(velocity_at_contact, normal);

    if (relative_velocity_normal > -0.001f)
        return false;

    const glm::vec3 perpendicular = LEti::Math::cross_product(vec_to_point, normal);
    const float effectiveMassDenom = rb->mass_inverse() +
                                     LEti::Math::dot_product(perpendicular, rb->inertia_tensor_inverse() * perpendicular);

    if (effectiveMassDenom <= 0.0f)
        return false;

    const float impulse_along_normal = -2.0f * relative_velocity_normal / effectiveMassDenom;

    glm::vec3 total_impulse = normal * impulse_along_normal;

    if (fabsf(impulse_along_normal) > 0.0001f)
    {
        glm::vec3 tangent_velocity = velocity_at_contact - normal * relative_velocity_normal;
        const float tangent_speed = length(tangent_velocity);

        if (tangent_speed > 0.0001f)
        {
            LEti::Math::shrink_vector_to_1(tangent_velocity);

            const glm::vec3 r_cross_t = LEti::Math::cross_product(vec_to_point, tangent_velocity);
            const float tangent_mass_denom = rb->mass_inverse() +
                                           LEti::Math::dot_product(r_cross_t, rb->inertia_tensor_inverse() * r_cross_t);

            if (tangent_mass_denom > 0.0001f)
            {
                const float full_tangent_impulse = -tangent_speed / tangent_mass_denom;

                const float max_tangent_impulse = fabsf(impulse_along_normal);
                const float tangent_impulse = glm::clamp(full_tangent_impulse, -max_tangent_impulse, max_tangent_impulse);

                total_impulse += tangent_velocity * tangent_impulse;
            }
        }
    }

    glm::vec3 velocity = rb->velocity() + (total_impulse * rb->mass_inverse());
    glm::vec3 angular_velocity = rb->angular_velocity() + (rb->inertia_tensor_inverse() * LEti::Math::cross_product(vec_to_point, total_impulse));

    rb->set_velocity(velocity);
    rb->set_angular_velocity(angular_velocity);

    float ke_after = M_calculate_kinetic_energy(rb->velocity(), rb->angular_velocity(), rb->mass(), rb->moment_of_inertia());

    if(ke_after > 0.0f)
    {
        float ratio_sqrt = sqrtf(ke_before / ke_after);

        rb->set_velocity( rb->velocity() * ratio_sqrt );
        rb->set_angular_velocity( rb->angular_velocity() * ratio_sqrt );
    }

    m_default_collision_resolution.resolve(_id, _dt);

    return true;
}



bool Collision_Resolution__Rigid_Body_3D::resolve(const LPhys::Intersection_Data &_id, float _dt)
{
    if(!_id.first->is_static() && !_id.second->is_static())
        return M_resolve_dynamic_vs_dynamic(_id, _dt);

    if(_id.first->is_static() && _id.second->is_static())
        return false;

    return M_resolve_dynamic_vs_static(_id, _dt);
}
