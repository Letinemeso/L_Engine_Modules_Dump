#pragma once

#include <Module.h>


namespace LMD
{

    class Directional_Movement_Module : public LEti::Module
    {
    public:
        INIT_VARIABLE(LMD::Directional_Movement_Module, LEti::Module)

    private:
        glm::vec3 m_direction = {0.0f, 0.0f, 0.0f};
        float m_velocity = 0.0f;

        glm::vec3 m_stride_per_second = { 0.0f, 0.0f, 0.0f };

    public:
        inline const glm::vec3& direction() const { return m_direction; }
        inline float velocity() const { return m_velocity; }

        inline const glm::vec3& stride_per_second() const { return m_stride_per_second; }

    public:
        void set_direction(const glm::vec3& _value);
        void set_velocity(float _value);

    public:
        void update(float _dt) override;

    };


    class Directional_Movement_Module_Stub : public LEti::Module_Stub
    {
    public:
        INIT_VARIABLE(LMD::Directional_Movement_Module_Stub, LEti::Module_Stub)

        INIT_FIELDS
        ADD_FIELD(glm::vec3, direction)
        ADD_FIELD(float, velocity)
        FIELDS_END

    public:
        glm::vec3 direction = { 0.0f, 0.0f, 0.0f };
        float velocity = 0.0f;

    public:
        INIT_BUILDER_STUB(Directional_Movement_Module)

    };

}
