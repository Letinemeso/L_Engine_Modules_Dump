#pragma once

#include <Module.h>


namespace LMD
{

    class Rigid_Body_Setup_Module : public LEti::Module
    {
    public:
        INIT_VARIABLE(LMD::Rigid_Body_Setup_Module, LEti::Module)

    private:
        glm::vec3 m_min_velocity = {0.0f, 0.0f, 0.0f};
        glm::vec3 m_max_velocity = {0.0f, 0.0f, 0.0f};

        float m_min_angular_velocity = 0.0f;
        float m_max_angular_velocity = 0.0f;

    public:
        inline void set_min_velocity(const glm::vec3& _value) { m_min_velocity = _value; }
        inline void set_max_velocity(const glm::vec3& _value) { m_max_velocity = _value; }

        inline void set_min_angular_velocity(float _value) { m_min_angular_velocity = _value; }
        inline void set_max_angular_velocity(float _value) { m_max_angular_velocity = _value; }

    private:
        void M_on_parent_object_set() override;

    public:
        void update(float _dt) override { }

    };


    class Rigid_Body_Setup_Module_Stub : public LEti::Module_Stub
    {
    public:
        INIT_VARIABLE(LMD::Rigid_Body_Setup_Module_Stub, LEti::Module_Stub)

        INIT_FIELDS
        ADD_FIELD(glm::vec3, min_velocity)
        ADD_FIELD(glm::vec3, max_velocity)
        ADD_FIELD(float, min_angular_velocity)
        ADD_FIELD(float, max_angular_velocity)
        FIELDS_END

    public:
        glm::vec3 min_velocity = {0.0f, 0.0f, 0.0f};
        glm::vec3 max_velocity = {0.0f, 0.0f, 0.0f};

        float min_angular_velocity = 0.0f;
        float max_angular_velocity = 0.0f;

    public:
        INIT_BUILDER_STUB(Rigid_Body_Setup_Module)

    };

}
