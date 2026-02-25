#include <Misc_Modules/Debug_Movement_Module.h>

#include <glfw3.h>

#include <Window/Window_Controller.h>

using namespace LMD;


Debug_Movement_Module::Debug_Movement_Module()
{
    static bool s_initialized_log_level = false;
    if(!s_initialized_log_level)
    {
        L_CREATE_LOG_LEVEL(log_level());
        s_initialized_log_level = true;
    }

    L_LOG(log_level(), "Created Debug_Movement_Module!");
}

Debug_Movement_Module::~Debug_Movement_Module()
{
    L_LOG(log_level(), "Destroyed Debug_Movement_Module!");
}



std::string Debug_Movement_Module::M_vec_to_string(const glm::vec3& _vec) const
{
    return "x: " + std::to_string(_vec.x) + " y: " + std::to_string(_vec.y) + " z: " + std::to_string(_vec.z);
}


void Debug_Movement_Module::M_process_movement(float _dt)
{
    if(!m_is_moving)
        return;

    bool input = false;

    if(LR::Window_Controller::instance().is_key_down(GLFW_KEY_RIGHT))
    {
        transformation_data()->move({ m_movement_speed * _dt, 0.0f, 0.0f });
        input = true;
    }
    if(LR::Window_Controller::instance().is_key_down(GLFW_KEY_LEFT))
    {
        transformation_data()->move({ -m_movement_speed * _dt, 0.0f, 0.0f });
        input = true;
    }
    if(LR::Window_Controller::instance().is_key_down(GLFW_KEY_UP))
    {
        transformation_data()->move({ 0.0f, 0.0f, -m_movement_speed * _dt });
        input = true;
    }
    if(LR::Window_Controller::instance().is_key_down(GLFW_KEY_DOWN))
    {
        transformation_data()->move({ 0.0f, 0.0f, m_movement_speed * _dt });
        input = true;
    }
    if(LR::Window_Controller::instance().is_key_down(GLFW_KEY_PAGE_UP))
    {
        transformation_data()->move({ 0.0f, m_movement_speed * _dt, 0.0f });
        input = true;
    }
    if(LR::Window_Controller::instance().is_key_down(GLFW_KEY_PAGE_DOWN))
    {
        transformation_data()->move({ 0.0f, -m_movement_speed * _dt, 0.0f });
        input = true;
    }

    if(input)
    {
        L_LOG(log_level(), "Moved object, current position: " + M_vec_to_string(transformation_data()->position()));
    }
}

void Debug_Movement_Module::M_process_rotation(float _dt)
{
    if(m_is_moving)
        return;

    bool input = false;

    if(LR::Window_Controller::instance().is_key_down(GLFW_KEY_RIGHT))
    {
        transformation_data()->rotate({ 0.0f, 0.0f, m_rotation_speed * _dt });
        input = true;
    }
    if(LR::Window_Controller::instance().is_key_down(GLFW_KEY_LEFT))
    {
        transformation_data()->rotate({ 0.0f, 0.0f, -m_rotation_speed * _dt });
        input = true;
    }
    if(LR::Window_Controller::instance().is_key_down(GLFW_KEY_UP))
    {
        transformation_data()->rotate({ m_rotation_speed * _dt, 0.0f, 0.0f });
        input = true;
    }
    if(LR::Window_Controller::instance().is_key_down(GLFW_KEY_DOWN))
    {
        transformation_data()->rotate({ -m_rotation_speed * _dt, 0.0f, 0.0f });
        input = true;
    }
    if(LR::Window_Controller::instance().is_key_down(GLFW_KEY_PAGE_UP))
    {
        transformation_data()->rotate({ 0.0f, m_rotation_speed * _dt, 0.0f });
        input = true;
    }
    if(LR::Window_Controller::instance().is_key_down(GLFW_KEY_PAGE_DOWN))
    {
        transformation_data()->rotate({ 0.0f, -m_rotation_speed * _dt, 0.0f });
        input = true;
    }

    if(input)
    {
        L_LOG(log_level(), "Rotated object, current rotation: " + M_vec_to_string(transformation_data()->position()));
    }

}



void Debug_Movement_Module::update(float _dt)
{
    if(LR::Window_Controller::instance().key_was_pressed(GLFW_KEY_RIGHT_CONTROL))
    {
        m_is_moving = !m_is_moving;
        L_LOG(log_level(), std::string("Switching mode to ") + (m_is_moving ? "movement" : "rotation"));
    }

    M_process_movement(_dt);
    M_process_rotation(_dt);
}
