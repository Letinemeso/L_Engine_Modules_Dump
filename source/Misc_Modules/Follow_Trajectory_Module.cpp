#include <Misc_Modules/Follow_Trajectory_Module.h>

#include <Stuff/Math_Stuff.h>

using namespace LMD;


Follow_Trajectory_Module::Follow_Trajectory_Module()
{
    m_trajectory.set_on_update_func(
        [this](const glm::vec3& _point)
        {
            L_ASSERT(transformation_data());

            transformation_data()->set_position(_point + m_constant_offset);
        });
}

Follow_Trajectory_Module::~Follow_Trajectory_Module()
{

}



void Follow_Trajectory_Module::setup_trajectory(const LDS::List<glm::vec3>& _points_list)
{
    L_ASSERT(_points_list.size() > 1);
    L_ASSERT(m_speed > 0.0f);

    m_trajectory.clear();

    LDS::List<glm::vec3>::Const_Iterator it = _points_list.begin();

    glm::vec first_point = *it;

    m_trajectory.add_frame(0.0f, first_point);

    float time_offset = 0.0f;

    while(!it.end_reached())
    {
        LDS::List<glm::vec3>::Const_Iterator next_it = it;
        ++next_it;
        if(next_it.end_reached())
            break;

        float distance = LST::Math::vector_length(*next_it - *it);
        time_offset += distance / m_speed;

        m_trajectory.add_frame(time_offset, *next_it);

        it = next_it;
    }

    if(m_should_run_in_loop)
    {
        float distance = LST::Math::vector_length(*it - first_point);
        time_offset += distance / m_speed;
        m_trajectory.add_frame(time_offset, first_point);
    }

    m_trajectory.set_run_in_loop(m_should_run_in_loop);
    m_trajectory.start();
}



void Follow_Trajectory_Module::update(float _dt)
{
    if(!m_active)
        return;

    m_trajectory.update(_dt);
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Follow_Trajectory_Module_Stub)

BUILDER_STUB_INITIALIZATION_FUNC(Follow_Trajectory_Module_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_should_run_in_loop(should_run_in_loop);
    product->set_active(autostart);
    product->set_speed(speed);
    product->set_constant_offset(constant_offset);
    if(trajectory.size() > 1)
        product->setup_trajectory(trajectory);
}
