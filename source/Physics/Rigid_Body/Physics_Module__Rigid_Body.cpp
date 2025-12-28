#include <Physics/Rigid_Body/Physics_Module__Rigid_Body.h>

#include <Math_Stuff.h>

using namespace LMD;


LPhys::Physical_Model* Physics_Module__Rigid_Body::M_create_physical_model() const
{
    return new Rigid_Body_Physical_Model;
}



void Physics_Module__Rigid_Body::set_mass(float _mass)
{
    Rigid_Body_Physical_Model* physical_model = (Rigid_Body_Physical_Model*)get_physical_model();

    physical_model->set_mass(_mass);
}



float Physics_Module__Rigid_Body::mass() const
{
    Rigid_Body_Physical_Model* physical_model = (Rigid_Body_Physical_Model*)get_physical_model();

    return physical_model->mass();
}

float Physics_Module__Rigid_Body::mass_inverse() const
{
    Rigid_Body_Physical_Model* physical_model = (Rigid_Body_Physical_Model*)get_physical_model();

    if(physical_model->mass() > 1e-6f)
        return 1.0f / physical_model->mass();

    return 0.0f;
}

float Physics_Module__Rigid_Body::moment_of_inertia() const
{
    Rigid_Body_Physical_Model* physical_model = (Rigid_Body_Physical_Model*)get_physical_model();

    return physical_model->moment_of_inertia();
}

float Physics_Module__Rigid_Body::moment_of_inertia_inverse() const
{
    Rigid_Body_Physical_Model* physical_model = (Rigid_Body_Physical_Model*)get_physical_model();

    if(physical_model->moment_of_inertia() > 1e-6f)
        return 1.0f / physical_model->moment_of_inertia();

    return 0.0f;
}

const glm::mat3x3& Physics_Module__Rigid_Body::inertia_tensor() const
{
    Rigid_Body_Physical_Model* physical_model = (Rigid_Body_Physical_Model*)get_physical_model();

    return physical_model->inertia_tensor();
}

const glm::mat3x3& Physics_Module__Rigid_Body::inertia_tensor_inverse() const
{
    Rigid_Body_Physical_Model* physical_model = (Rigid_Body_Physical_Model*)get_physical_model();

    return physical_model->inertia_tensor_inverse();
}

const glm::vec3& Physics_Module__Rigid_Body::center_of_mass() const
{
    Rigid_Body_Physical_Model* physical_model = (Rigid_Body_Physical_Model*)get_physical_model();

    return physical_model->center_of_mass();
}



void Physics_Module__Rigid_Body::update(float _dt)
{
    L_ASSERT(get_physical_model() && get_physical_model_prev_state() && transformation_data());

    transformation_data()->move(m_velocity * _dt);

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





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Physics_Module_Stub__Rigid_Body)

BUILDER_STUB_INITIALIZATION_FUNC(Physics_Module_Stub__Rigid_Body)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_mass(mass);
}
