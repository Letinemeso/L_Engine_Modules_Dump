#include <Physics/Rigid_Body/Physics_Module__Rigid_Body.h>

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

float Physics_Module__Rigid_Body::moment_of_inertia() const
{
    Rigid_Body_Physical_Model* physical_model = (Rigid_Body_Physical_Model*)get_physical_model();

    return physical_model->moment_of_inertia();
}

const glm::vec3& Physics_Module__Rigid_Body::center_of_mass() const
{
    Rigid_Body_Physical_Model* physical_model = (Rigid_Body_Physical_Model*)get_physical_model();

    return physical_model->center_of_mass();
}



void Physics_Module__Rigid_Body::update(float _dt)
{
    L_ASSERT(get_physical_model() && get_physical_model_prev_state() && transformation_data());

    transformation_data()->move(velocity() * _dt);
    transformation_data()->rotate({0.0f, 0.0f, angular_velocity() * _dt});

    Physics_Module__Mesh::update(_dt);
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Physics_Module_Stub__Rigid_Body)

BUILDER_STUB_INITIALIZATION_FUNC(Physics_Module_Stub__Rigid_Body)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_mass(mass);
}
