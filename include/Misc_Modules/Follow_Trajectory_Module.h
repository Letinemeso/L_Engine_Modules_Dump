#pragma once

#include <Data_Structures/List.h>
#include <Stuff/Key_Frame_Animation.h>

#include <Module.h>


namespace LMD
{

    class Follow_Trajectory_Module : public LEti::Module
    {
    public:
        INIT_VARIABLE(LMD::Follow_Trajectory_Module, LEti::Module);

    public:
        using Trajectory_KFA = LST::Key_Frame_Animation<glm::vec3>;

    private:
        bool m_should_run_in_loop = true;
        bool m_active = true;
        float m_speed = 0.0f;
        glm::vec3 m_constant_offset{0.0f, 0.0f, 0.0f};
        Trajectory_KFA m_trajectory;

    public:
        Follow_Trajectory_Module();
        ~Follow_Trajectory_Module();

    public:
        inline void set_should_run_in_loop(bool _value) { m_should_run_in_loop = _value; }
        inline bool should_run_in_loop() const { return m_should_run_in_loop; }

        inline void set_active(bool _value) { m_active = _value; }
        inline bool active() const { return m_active; }

        inline void set_speed(float _value) { m_speed = _value; }
        inline float speed() const { return m_speed; }

        inline void set_constant_offset(const glm::vec3& _value) { m_constant_offset = _value; }
        inline const glm::vec3& constant_offset() const { return m_constant_offset; }

        inline bool trajectory_active() const { return m_trajectory.active(); }

    public:
        void setup_trajectory(const LDS::List<glm::vec3>& _points_list);

    public:
        void update(float _dt) override;

    };


    class Follow_Trajectory_Module_Stub : public LEti::Module_Stub
    {
    public:
        INIT_VARIABLE(LMD::Follow_Trajectory_Module_Stub, LEti::Module_Stub);

        INIT_FIELDS
        ADD_FIELD(bool, should_run_in_loop)
        ADD_FIELD(bool, autostart)
        ADD_FIELD(float, speed)
        ADD_FIELD(glm::vec3, constant_offset)
        ADD_FIELD(LDS::List<glm::vec3>, trajectory)
        FIELDS_END

    public:
        bool should_run_in_loop = true;
        bool autostart = true;
        float speed = 0.0f;
        glm::vec3 constant_offset{0.0f, 0.0f, 0.0f};
        LDS::List<glm::vec3> trajectory;

    public:
        INIT_BUILDER_STUB(Follow_Trajectory_Module);

    };

}
