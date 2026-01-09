#include <Misc_Modules/Rigid_Body_Setup_Module.h>

#include <Stuff/Math_Stuff.h>
#include <Object.h>

#include <Physics/Rigid_Body/Physics_Module__Rigid_Body.h>

using namespace LMD;


void Rigid_Body_Setup_Module::M_on_parent_object_set()
{
    Physics_Module__Rigid_Body* rb = parent_object()->get_module_of_type<Physics_Module__Rigid_Body>();
    L_ASSERT(rb);

    glm::vec3 random_velocity = LST::Math::random_vec3(m_min_velocity, m_max_velocity);
    rb->set_velocity(random_velocity);

    constexpr glm::vec3 Limit_Vector = {1.0f, 1.0f, 1.0f};
    glm::vec3 random_angular_velocity = LST::Math::random_vec3(-Limit_Vector, Limit_Vector);
    float random_angular_velocity_value = LST::Math::random_number_float(m_min_angular_velocity, m_max_angular_velocity);
    LST::Math::extend_vector_to_length(random_angular_velocity, random_angular_velocity_value);

    rb->set_angular_velocity(random_angular_velocity);
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Rigid_Body_Setup_Module_Stub)

BUILDER_STUB_INITIALIZATION_FUNC(Rigid_Body_Setup_Module_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_min_velocity(min_velocity);
    product->set_max_velocity(max_velocity);
    product->set_min_angular_velocity(min_angular_velocity);
    product->set_max_angular_velocity(max_angular_velocity);
}
