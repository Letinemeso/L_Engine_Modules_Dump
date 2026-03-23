#include <Physics/Rigid_Body/Collision_Resolution__Rigid_Body_3D.h>

#include <Stuff/Math_Stuff.h>

using namespace LMD;


namespace LMD
{
    float compute_kinetic_energy(const Physics_Module__Rigid_Body* _rb)
    {
        float linear = 0.5f * _rb->mass() * LST::Math::dot_product(_rb->velocity(), _rb->velocity());
        float angular = 0.5f * LST::Math::dot_product(_rb->angular_velocity(), _rb->inertia_tensor() * _rb->angular_velocity());

        return linear + angular;
    }
}



bool Collision_Resolution__Rigid_Body_3D::M_resolve_dynamic_vs_dynamic(const LPhys::Intersection_Data& _id, float _dt)
{
    Physics_Module__Rigid_Body* pm1 = LV::cast_variable<Physics_Module__Rigid_Body>(_id.first);
    Physics_Module__Rigid_Body* pm2 = LV::cast_variable<Physics_Module__Rigid_Body>(_id.second);

    if(!pm1 || !pm2)
        return false;

    if(!m_default_collision_resolution.resolve(_id, _dt))
        return false;

    glm::vec3 vec_to_point_1 = _id.points[0] - pm1->center_of_mass();
    glm::vec3 vec_to_point_2 = _id.points[0] - pm2->center_of_mass();

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

    glm::vec3 normal = _id.normal;
    if(dynamic_pm == _id.second)
        normal *= -1.0f;

    LDS::Vector<glm::vec3> radius_vectors(_id.points.size());
    for(unsigned int i = 0; i < _id.points.size(); ++i)
        radius_vectors.push( _id.points[i] - rb->center_of_mass() );

    constexpr unsigned int Iterations_Amount = 10;
    for(unsigned int i_i = 0; i_i < Iterations_Amount; ++i_i)
    {
        for(unsigned int i = 0; i < _id.points.size(); ++i)
            M_resolve_dynamic_vs_static_single_point(rb, radius_vectors[i], normal);
    }

    m_default_collision_resolution.resolve(_id, _dt);

    return true;
}


void Collision_Resolution__Rigid_Body_3D::M_resolve_dynamic_vs_static_single_point(Physics_Module__Rigid_Body* _rb, const glm::vec3& _radius_vector, const glm::vec3& _contact_normal)
{
    glm::vec3 angular_velocity_component = LST::Math::cross_product(_rb->angular_velocity(), _radius_vector);
    glm::vec3 relative_velocity = _rb->velocity() + angular_velocity_component;

    float normal_velocity = LST::Math::dot_product(relative_velocity, _contact_normal);

    if (normal_velocity > 0.0f)
        return;

    glm::vec3 r_cross_n = LST::Math::cross_product(_radius_vector, _contact_normal);
    glm::vec3 angular_term = _rb->inertia_tensor_inverse() * r_cross_n;

    float effective_mass = _rb->mass_inverse() + LST::Math::dot_product(_contact_normal, LST::Math::cross_product(angular_term, _radius_vector));

    // float impulse_magnitude = -normal_velocity / effective_mass;

    float effective_restitution = 0.0f;
    if(normal_velocity > 0.3f)
        effective_restitution = _rb->restitution();

    float impulse_magnitude = -(1.0f + effective_restitution) * normal_velocity / effective_mass;

    if (impulse_magnitude < 0.0f)
        impulse_magnitude = 0.0f;

    glm::vec3 impulse = impulse_magnitude * _contact_normal;

    glm::vec3 velocity_increment = impulse * _rb->mass_inverse();
    glm::vec3 angular_velocity_increment = _rb->inertia_tensor_inverse() * LST::Math::cross_product(_radius_vector, impulse);

    float kinetic_energy_before = compute_kinetic_energy(_rb);

    _rb->apply_linear_impulse(velocity_increment);
    _rb->apply_rotation(angular_velocity_increment);

    float kinetic_energy_after = compute_kinetic_energy(_rb);

    if(kinetic_energy_after > kinetic_energy_before * 1.2f)
        std::cout << "kinetic energy significantly increased" << std::endl;

    M_apply_friction(_rb,  _radius_vector, _contact_normal, impulse_magnitude);

    float kinetic_energy_after_friction = compute_kinetic_energy(_rb);

    if(kinetic_energy_after_friction > kinetic_energy_after * 1.2f)
        std::cout << "kinetic energy significantly increased after friction application" << std::endl;
}

void Collision_Resolution__Rigid_Body_3D::M_apply_friction(Physics_Module__Rigid_Body* _rb, const glm::vec3& _radius_vector, const glm::vec3& _contact_normal, float _impulse_magnitude)
{
    glm::vec3 angular_velocity_component = LST::Math::cross_product(_rb->angular_velocity(), _radius_vector);
    glm::vec3 relative_velocity = _rb->velocity() + angular_velocity_component;

    glm::vec3 tangent_velocity = relative_velocity - LST::Math::dot_product(relative_velocity, _contact_normal) * _contact_normal;

    float tangent_speed_sq = LST::Math::vector_length_squared(tangent_velocity);

    if (tangent_speed_sq <= LST::Math::Float_Precision_Tolerance_Very_Risky)
        return;

    glm::vec3 tangent = tangent_velocity / sqrtf(tangent_speed_sq);

    float tangent_speed = LST::Math::dot_product(relative_velocity, tangent);

    glm::vec3 r_cross_t = LST::Math::cross_product(_radius_vector, tangent);

    glm::vec3 angular_term_t = _rb->inertia_tensor_inverse() * r_cross_t;

    float effective_mass_t = _rb->mass_inverse() + LST::Math::dot_product(tangent, LST::Math::cross_product(angular_term_t, _radius_vector));

    float friction_impulse_magnitude = -tangent_speed / effective_mass_t;

    float max_friction = _rb->traction() * _impulse_magnitude;
    friction_impulse_magnitude = glm::clamp(friction_impulse_magnitude, -max_friction, max_friction);

    glm::vec3 friction_impulse = friction_impulse_magnitude * tangent;

    glm::vec3 velocity_increment = friction_impulse * _rb->mass_inverse();
    glm::vec3 angular_velocity_increment = _rb->inertia_tensor_inverse() * LST::Math::cross_product(_radius_vector, friction_impulse);

    _rb->apply_linear_impulse(velocity_increment);
    _rb->apply_rotation(angular_velocity_increment);
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
