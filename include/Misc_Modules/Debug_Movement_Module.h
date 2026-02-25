#pragma once

#include <Module.h>


namespace LMD
{

    class Debug_Movement_Module : public LEti::Module
    {
    public:
        INIT_VARIABLE(LMD::Debug_Movement_Module, LEti::Module)

    private:
        float m_movement_speed = 0.0f;
        float m_rotation_speed = 0.0f;

    private:
        bool m_is_moving = true;

    public:
        Debug_Movement_Module();
        ~Debug_Movement_Module();

    public:
        inline void set_movement_speed(float _value) { m_movement_speed = _value; }
        inline void set_rotation_speed(float _value) { m_rotation_speed = _value; }

    public:
        inline const std::string& log_level() const { return get_estimated_type(); }

    private:
        std::string M_vec_to_string(const glm::vec3& _vec) const;

        void M_process_movement(float _dt);
        void M_process_rotation(float _dt);

    public:
        void update(float _dt) override;

    };


    class Debug_Movement_Module_Stub : public LEti::Module_Stub
    {
    public:
        INIT_VARIABLE(LMD::Debug_Movement_Module_Stub, LEti::Module_Stub)

        INIT_FIELDS
        ADD_FIELD(float, movement_speed)
        ADD_FIELD(float, rotation_speed)
        FIELDS_END

    public:
        float movement_speed = 0.0f;
        float rotation_speed = 0.0f;

    public:
        INIT_DEFAULT_BUILDER_STUB(Debug_Movement_Module)

        INIT_BUILDER_STUB_SETTERS
        ADD_BUILDER_STUB_SETTER(set_movement_speed, movement_speed)
        ADD_BUILDER_STUB_SETTER(set_rotation_speed, rotation_speed)
        BUILDER_STUB_SETTERS_END

    };

}
