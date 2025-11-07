#include <Misc_Modules/Directional_Movement_Module.h>

#include <Math_Stuff.h>

using namespace LMD;


void Directional_Movement_Module::set_direction(const glm::vec3& _value)
{
    m_direction = _value;
    LEti::Math::shrink_vector_to_1(m_direction);

    m_stride_per_second = m_direction * m_velocity;
}

void Directional_Movement_Module::set_velocity(float _value)
{
    m_velocity = _value;

    m_stride_per_second = m_direction * m_velocity;
}



void Directional_Movement_Module::update(float _dt)
{
    transformation_data()->move(m_stride_per_second * _dt);
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Directional_Movement_Module_Stub)

BUILDER_STUB_INITIALIZATION_FUNC(Directional_Movement_Module_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_direction(direction);
    product->set_velocity(velocity);
}
