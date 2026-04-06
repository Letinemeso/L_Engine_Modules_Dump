#include <Physics/Rigid_Body/Collision_Resolution__Rigid_Body_3D.h>

#include <Stuff/Math_Stuff.h>

#include <Physics/Rigid_Body/Rigid_Body_Utility.h>

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
    LPhys::Physics_Module* dynamic_pm = _id.first;
    LPhys::Physics_Module* static_pm = _id.second;

    if(_id.first->is_static())
    {
        dynamic_pm = _id.second;
        static_pm = _id.first;
    }

    L_ASSERT(!dynamic_pm->is_static());

    Physics_Module__Rigid_Body* rb = LV::cast_variable<Physics_Module__Rigid_Body>(dynamic_pm);
    if(!rb)
        return false;

    glm::vec3 normal = _id.normal;
    if(dynamic_pm == _id.second)
        normal *= -1.0f;

    Prep_Data prep_data = M_calculate_dvs_prep_data(dynamic_pm, static_pm, normal, _id.points);

    M_apply_old_dvs_data(rb, prep_data, normal);

    constexpr unsigned int Iterations_Amount = 10;
    for(unsigned int i_i = 0; i_i < Iterations_Amount; ++i_i)
    {
        for(unsigned int i = 0; i < _id.points.size(); ++i)
        {
            WS_Contact_Data& current_ws = *prep_data.new_ws_data[i];

            M_resolve_dynamic_vs_static_single_point(rb, prep_data.radius_vectors_0[i], normal, _id.depth, _dt, current_ws);
        }
    }

    m_default_collision_resolution.resolve(_id, _dt);

    return true;
}


LST::UInt128 Collision_Resolution__Rigid_Body_3D::M_construct_pair_key(const LPhys::Physics_Module* _pm_0, const LPhys::Physics_Module* _pm_1) const
{
    LST::UInt128::Part_Type part_0 = (LST::UInt128::Part_Type)_pm_0;
    LST::UInt128::Part_Type part_1 = (LST::UInt128::Part_Type)_pm_1;

    if(part_0 > part_1)
        return {part_0, part_1};
    return {part_1, part_0};
}

bool Collision_Resolution__Rigid_Body_3D::M_ws_data_compatible(const WS_Contact_Data& _data, const glm::vec3& _normal, const glm::vec3& _local_point_0, const glm::vec3& _local_point_1) const
{
    float distance_squared = LST::Math::vector_length_squared(_data.local_point_0 - _local_point_0);
    if(distance_squared > m_ws_local_point_distance_tolerance_squared)
        return false;
    distance_squared = LST::Math::vector_length_squared(_data.local_point_1 - _local_point_1);
    if(distance_squared > m_ws_local_point_distance_tolerance_squared)
        return false;

    constexpr float Acceptable_Normal_Difference = 0.9f;
    float normal_cos = LST::Math::dot_product(_normal, _data.normal);
    if(normal_cos < Acceptable_Normal_Difference)
        return false;

    return true;
}

glm::vec3 Collision_Resolution__Rigid_Body_3D::M_calculate_local_contact_point(const LPhys::Physics_Module* _pm, const glm::vec3& _contact_point) const
{
    return glm::inverse(_pm->transformation_data()->rotation_matrix())
           *
           glm::vec4(_contact_point - _pm->transformation_data()->position(), 0.0f);
}

Collision_Resolution__Rigid_Body_3D::WS_Contact_Data* Collision_Resolution__Rigid_Body_3D::M_find_ws_data(LDS::Vector<WS_Contact_Data>& _ws_data,
                                                                                                             const glm::vec3& _normal,
                                                                                                             const glm::vec3& _local_contact_point_0,
                                                                                                             const glm::vec3& _local_contact_point_1)
{
    for(unsigned int i = 0; i < _ws_data.size(); ++i)
    {
        WS_Contact_Data& ws_data = _ws_data[i];
        if(!M_ws_data_compatible(ws_data, _normal, _local_contact_point_0, _local_contact_point_1))
            continue;

        return &ws_data;
    }

    return nullptr;
}


Collision_Resolution__Rigid_Body_3D::Prep_Data Collision_Resolution__Rigid_Body_3D::M_calculate_dvs_prep_data(const LPhys::Physics_Module* _dynamic_pm,
                                                                                                              const LPhys::Physics_Module* _static_pm,
                                                                                                              const glm::vec3& _normal,
                                                                                                              const LDS::Vector<glm::vec3>& _contact_points)
{
    Prep_Data result;

    L_ASSERT(LV::cast_variable<Physics_Module__Rigid_Body>(_dynamic_pm));
    const Physics_Module__Rigid_Body* rb = LST::raw_cast<Physics_Module__Rigid_Body>(_dynamic_pm);

    LST::UInt128 search_key = M_construct_pair_key(_dynamic_pm, _static_pm);
    L_ASSERT(!m_current_intersections.find(search_key).is_ok());
    LDS::Vector<WS_Contact_Data>& current_ws_data = m_current_intersections.insert_and_get_iterator(search_key, {})->data;

    for(unsigned int i = 0; i < _contact_points.size(); ++i)
    {
        const glm::vec3& contact_point = _contact_points[i];

        result.radius_vectors_0.push( Rigid_Body_Utility::calculate_radius_vector(*rb, contact_point) );

        glm::vec3 local_contact_point_0 = M_calculate_local_contact_point(_dynamic_pm, contact_point);
        glm::vec3 local_contact_point_1 = M_calculate_local_contact_point(_static_pm, contact_point);

        current_ws_data.push({});
        WS_Contact_Data& ws_data = current_ws_data[current_ws_data.size() - 1];
        ws_data.local_point_0 = local_contact_point_0;
        ws_data.local_point_1 = local_contact_point_1;
        ws_data.normal = _normal;

        result.new_ws_data.push(&ws_data);
    }

    Warm_Starting_Data_Map::Iterator maybe_data_it = m_previous_intersections.find(search_key);
    if(!maybe_data_it.is_ok())
        return result;

    LDS::Vector<WS_Contact_Data>& old_ws_data = maybe_data_it->data;
    for(unsigned int i = 0; i < _contact_points.size(); ++i)
    {
        const WS_Contact_Data& ws_data = current_ws_data[i];
        const glm::vec3& local_contact_point_0 = ws_data.local_point_0;
        const glm::vec3& local_contact_point_1 = ws_data.local_point_1;

        result.old_ws_data.push( M_find_ws_data(old_ws_data, _normal, local_contact_point_0, local_contact_point_1) );
    }

    return result;
}


void Collision_Resolution__Rigid_Body_3D::M_apply_old_dvs_data(Physics_Module__Rigid_Body* _dynamic_rb, Prep_Data& _prep_data, const glm::vec3& _normal)
{
    for(unsigned int i = 0; i < _prep_data.old_ws_data.size(); ++i)
    {
        const WS_Contact_Data* ws_data = _prep_data.old_ws_data[i];
        if(!ws_data)
            continue;

        glm::vec3 normal_projected = ws_data->normal * LST::Math::dot_product(_normal, ws_data->normal);
        glm::vec3 impulse_to_apply = ws_data->accumulated_normal_impulse * normal_projected;
        M_apply_impulse(_dynamic_rb, _prep_data.radius_vectors_0[i], impulse_to_apply);

        _prep_data.new_ws_data[i]->accumulated_normal_impulse = LST::Math::vector_length(impulse_to_apply);
    }
}


void Collision_Resolution__Rigid_Body_3D::M_resolve_dynamic_vs_static_single_point(Physics_Module__Rigid_Body* _rb, const glm::vec3& _radius_vector,
                                                                                   const glm::vec3& _contact_normal, float _depth, float _dt, WS_Contact_Data& _ws_data)
{
    glm::vec3 relative_velocity = Rigid_Body_Utility::calculate_point_velocity(*_rb, _radius_vector);

    float normal_velocity = LST::Math::dot_product(relative_velocity, _contact_normal);

    glm::vec3 r_cross_n = LST::Math::cross_product(_radius_vector, _contact_normal);
    glm::vec3 angular_term = _rb->inertia_tensor_inverse() * r_cross_n;

    float effective_mass = _rb->mass_inverse() + LST::Math::dot_product(_contact_normal, LST::Math::cross_product(angular_term, _radius_vector));

    // if (normal_velocity > 0.0f)
    // {
    //     float impulse_magnitude = -(normal_velocity) / effective_mass;

    //     float old_accumulated_impulse = _ws_data.accumulated_normal_impulse;
    //     _ws_data.accumulated_normal_impulse = std::max(old_accumulated_impulse + impulse_magnitude, 0.0f);
    //     float impulse_delta = _ws_data.accumulated_normal_impulse - old_accumulated_impulse;

    //     glm::vec3 impulse = impulse_delta * _contact_normal;
    //     M_apply_impulse(_rb, _radius_vector, impulse);

    //     return;
    // }

    constexpr float Min_Normal_Velocity = 0.3f;
    float effective_restitution = 0.0f;
    if(normal_velocity < -Min_Normal_Velocity)
        effective_restitution = _rb->restitution();

    constexpr float Baumgarte = 0.005f;
    constexpr float Slope = 0.01f;

    float bias = 0.0f;

    if (_depth > Slope)
        bias = Baumgarte * (_depth - Slope) / _dt;

    float impulse_magnitude = -(normal_velocity * (1.0f + effective_restitution) + bias) / effective_mass;

    float old_accumulated_impulse = _ws_data.accumulated_normal_impulse;
    _ws_data.accumulated_normal_impulse = std::max(old_accumulated_impulse + impulse_magnitude, 0.0f);
    float impulse_delta = _ws_data.accumulated_normal_impulse - old_accumulated_impulse;

    glm::vec3 impulse = impulse_delta * _contact_normal;
    M_apply_impulse(_rb, _radius_vector, impulse);

    M_apply_friction(_rb, _radius_vector, _contact_normal, _ws_data.accumulated_normal_impulse);
}

void Collision_Resolution__Rigid_Body_3D::M_apply_impulse(Physics_Module__Rigid_Body* _rb, const glm::vec3& _radius_vector, const glm::vec3& _impulse)
{
    glm::vec3 velocity_increment = _impulse * _rb->mass_inverse();
    glm::vec3 angular_velocity_increment = _rb->inertia_tensor_inverse() * LST::Math::cross_product(_radius_vector, _impulse);

    _rb->apply_linear_impulse(velocity_increment);
    _rb->apply_rotation(angular_velocity_increment);
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



void Collision_Resolution__Rigid_Body_3D::on_before_pass()
{
    constexpr unsigned int Max_Frames_Since_Contact = 2;

    Warm_Starting_Data_Map::Iterator old_data_it = m_previous_intersections.iterator();
    while(!old_data_it.end_reached())
    {
        Warm_Starting_Data& ws_data = *old_data_it;
        if(ws_data.frames_since > Max_Frames_Since_Contact)
        {
            old_data_it = m_previous_intersections.erase_and_iterate_forward(old_data_it);
            continue;
        }

        ++old_data_it->frames_since;
        ++old_data_it;
    }

    for(Warm_Starting_Data_Map::Iterator it = m_current_intersections.iterator(); !it.end_reached(); ++it)
    {
        old_data_it = m_previous_intersections.find_or_insert(it.key());
        old_data_it->frames_since = 0;
        old_data_it->data = LST::move(it->data);
    }

    m_current_intersections.clear();
}

bool Collision_Resolution__Rigid_Body_3D::resolve(const LPhys::Intersection_Data &_id, float _dt)
{
    if(!_id.first->is_static() && !_id.second->is_static())
        return M_resolve_dynamic_vs_dynamic(_id, _dt);

    if(_id.first->is_static() && _id.second->is_static())
        return false;

    return M_resolve_dynamic_vs_static(_id, _dt);
}
