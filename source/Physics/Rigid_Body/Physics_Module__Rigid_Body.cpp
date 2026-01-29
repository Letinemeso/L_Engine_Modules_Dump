#include <Physics/Rigid_Body/Physics_Module__Rigid_Body.h>

#include <Stuff/Math_Stuff.h>

using namespace LMD;


LPhys::Physical_Model* Physics_Module__Rigid_Body::M_create_physical_model() const
{
    return new Rigid_Body_Physical_Model;
}



void Physics_Module__Rigid_Body::M_debug_check_vec3(const glm::vec3& _vec) const
{
    L_DEBUG_FUNC_NOARG([&]()
    {
        for(unsigned int i = 0; i < 3; ++i)
            L_ASSERT(!std::isnan(_vec[i]));
    });
}



void Physics_Module__Rigid_Body::set_mass(float _mass)
{
    cast_physical_model()->set_mass(_mass);
}

void Physics_Module__Rigid_Body::recalculate_raw_data()
{
    cast_physical_model()->recalculate_raw_data();
}



float Physics_Module__Rigid_Body::mass() const
{
    return cast_physical_model()->mass();
}

float Physics_Module__Rigid_Body::mass_inverse() const
{
    if(cast_physical_model()->mass() > 1e-6f)
        return 1.0f / cast_physical_model()->mass();

    return 0.0f;
}

float Physics_Module__Rigid_Body::moment_of_inertia() const
{
    return cast_physical_model()->moment_of_inertia();
}

float Physics_Module__Rigid_Body::moment_of_inertia_inverse() const
{
    if(cast_physical_model()->moment_of_inertia() > 1e-6f)
        return 1.0f / cast_physical_model()->moment_of_inertia();

    return 0.0f;
}

const glm::mat3x3& Physics_Module__Rigid_Body::inertia_tensor() const
{
    return m_inertia_tensor;
}

const glm::mat3x3& Physics_Module__Rigid_Body::inertia_tensor_inverse() const
{
    return m_inertia_tensor_inverse;
}

const glm::vec3& Physics_Module__Rigid_Body::center_of_mass() const
{
    return cast_physical_model()->center_of_mass();
}



glm::vec3 Physics_Module__Rigid_Body::M_calculate_world_position(const glm::vec3& _center_of_mass) const
{
    glm::vec4 offset = {cast_physical_model()->raw_center_of_mass(), 1.0f};
    offset = (transformation_data()->rotation_matrix() * transformation_data()->scale_matrix()) * offset;

    glm::vec3 result = _center_of_mass - glm::vec3(offset);
    return result;
}

glm::vec3 Physics_Module__Rigid_Body::M_calculate_center_of_mass() const
{
    glm::vec4 offset = {cast_physical_model()->raw_center_of_mass(), 1.0f};
    offset = (transformation_data()->rotation_matrix() * transformation_data()->scale_matrix()) * offset;

    glm::vec3 result = transformation_data()->position() + glm::vec3(offset);
    return result;
}



void Physics_Module__Rigid_Body::update(float _dt)
{
    if(!can_collide())
        return;

    L_ASSERT(get_physical_model() && get_physical_model_prev_state() && transformation_data());

    glm::vec3 center_of_mass = M_calculate_center_of_mass();
    center_of_mass += m_velocity * _dt;

    float angle = LST::Math::vector_length(m_angular_velocity);
    if (angle > 0.0f)
    {
        glm::vec3 axis = m_angular_velocity / angle;
        float half_angle = 0.5f * angle * _dt;

        const glm::quat& current_rotation_quat = transformation_data()->rotation();
        glm::quat rotation_quat = glm::quat(glm::cos(half_angle), axis * glm::sin(half_angle));

        glm::quat new_rotation_quat = glm::normalize(rotation_quat * current_rotation_quat);
        glm::vec3 new_euler_angles = LST::Math::calculate_angles(new_rotation_quat);

        transformation_data()->set_rotation( new_euler_angles );
    }

    glm::mat3x3 rotation_matrix = transformation_data()->rotation_matrix();
    m_inertia_tensor = rotation_matrix * cast_physical_model()->inertia_tensor_raw() * glm::transpose(rotation_matrix);
    m_inertia_tensor *= m_moment_of_inertia_scale;
    m_inertia_tensor_inverse = glm::inverse(m_inertia_tensor);

    glm::vec3 new_position = M_calculate_world_position(center_of_mass);
    transformation_data()->set_position(new_position);

    Physics_Module__Mesh::update(_dt);
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Physics_Module_Stub__Rigid_Body)

BUILDER_STUB_INITIALIZATION_FUNC(Physics_Module_Stub__Rigid_Body)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_mass(mass);
    product->set_restitution(restitution);
    product->set_moment_of_inertia_scale(moment_of_inertia_scale);
    product->set_traction(traction);

    product->recalculate_raw_data();
}
