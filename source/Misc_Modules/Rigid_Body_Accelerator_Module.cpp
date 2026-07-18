#include <Misc_Modules/Rigid_Body_Accelerator_Module.h>

#include <Object.h>

using namespace LMD;


void Rigid_Body_Accelerator_Module::M_on_parent_object_set()
{
    m_rigid_body = parent_object()->get_module_of_type<LMD::Physics_Module__Rigid_Body>();
    L_ASSERT(m_rigid_body);
}



void Rigid_Body_Accelerator_Module::update(float _dt)
{
    glm::vec3 additional_velocity = m_acceleration * _dt;

    if(m_rotate_with_object)
        additional_velocity = transformation_data()->rotation() * additional_velocity;

    m_rigid_body->apply_linear_impulse(additional_velocity);
}
