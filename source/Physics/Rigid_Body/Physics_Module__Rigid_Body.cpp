#include <Physics/Rigid_Body/Physics_Module__Rigid_Body.h>

#include <Math_Stuff.h>

using namespace LMD;


LPhys::Physical_Model* Physics_Module__Rigid_Body::M_create_physical_model() const
{
    return new Rigid_Body_Physical_Model;
}



void Physics_Module__Rigid_Body::set_mass(float _mass)
{
    cast_physical_model()->set_mass(_mass);
}

void Physics_Module__Rigid_Body::recalculate_raw_center_of_mass()
{
    cast_physical_model()->recalculate_raw_center_of_mass();
}

void Physics_Module__Rigid_Body::set_center_of_mass_position(const glm::vec3& _position)
{
    m_center_of_mass_position = _position;

    glm::vec3 new_position = M_calculate_world_position();
    transformation_data()->set_position(new_position);
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
    return cast_physical_model()->inertia_tensor();
}

const glm::mat3x3& Physics_Module__Rigid_Body::inertia_tensor_inverse() const
{
    return cast_physical_model()->inertia_tensor_inverse();
}

const glm::vec3& Physics_Module__Rigid_Body::center_of_mass() const
{
    return cast_physical_model()->center_of_mass();
}



glm::vec3 Physics_Module__Rigid_Body::M_calculate_world_position() const
{
    glm::vec4 offset = {cast_physical_model()->raw_center_of_mass(), 1.0f};
    offset = (transformation_data()->rotation_matrix() * transformation_data()->scale_matrix()) * offset;

    glm::vec3 result = m_center_of_mass_position - glm::vec3(offset);
    return result;
}



void Physics_Module__Rigid_Body::update(float _dt)
{
    L_ASSERT(get_physical_model() && get_physical_model_prev_state() && transformation_data());

    m_center_of_mass_position += m_velocity * _dt;

    glm::vec3 new_position = M_calculate_world_position();
    transformation_data()->set_position(new_position);

    glm::quat rotation_quat = {
        0.5f * m_angular_velocity.x * _dt,
        0.5f * m_angular_velocity.y * _dt,
        0.5f * m_angular_velocity.z * _dt,
        0.0f
    };
    glm::quat current_quat = LEti::Math::calculate_rotation_quaternion(transformation_data()->rotation());
    glm::quat new_rotation_quat = glm::normalize(current_quat + rotation_quat * current_quat);

    glm::vec3 new_euler_angles = LEti::Math::calculate_angles(new_rotation_quat);
    transformation_data()->set_rotation(new_euler_angles);

    Physics_Module__Mesh::update(_dt);
}


void Physics_Module__Rigid_Body::apply_data_after_collisions()
{
    if(m_transformations_after_collisions.size() == 0)
        return;

    glm::vec3 average_rotation(0.0f, 0.0f, 0.0f);
    glm::vec3 average_scale(0.0f, 0.0f, 0.0f);

    glm::vec3 largest_movement_vec(0.0f, 0.0f, 0.0f);

    for(Transformations_List::Iterator it = m_transformations_after_collisions.begin(); !it.end_reached(); ++it)
    {
        const LEti::Transformation_Data& transformation = *it;

        glm::vec3 movement_vec = transformation.position() - transformation_data()->position();

        for(unsigned int i = 0; i < 3; ++i)
        {
            if(fabs(largest_movement_vec[i]) >= fabs(movement_vec[i]))
                continue;

            largest_movement_vec[i] = movement_vec[i];
        }

        average_rotation += transformation.rotation();
        average_scale += transformation.scale();
    }

    float multiplier = 1.0f / (float)(m_transformations_after_collisions.size());

    average_rotation *= multiplier;
    average_scale *= multiplier;

    m_center_of_mass_position += largest_movement_vec;
    glm::vec3 new_position = M_calculate_world_position();
    transformation_data()->set_position(new_position);

    transformation_data()->set_rotation(average_rotation);
    transformation_data()->set_scale(average_scale);

    m_transformations_after_collisions.clear();

    Physics_Module__Mesh::update(0.0f);
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Physics_Module_Stub__Rigid_Body)

BUILDER_STUB_INITIALIZATION_FUNC(Physics_Module_Stub__Rigid_Body)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_mass(mass);

    product->recalculate_raw_center_of_mass();
}
