#pragma once

#include <Module.h>

#include <Physics/Rigid_Body/Physics_Module__Rigid_Body.h>


namespace LMD
{

    class Rigid_Body_Accelerator_Module : public LEti::Module
    {
    public:
        INIT_VARIABLE(LMD::Rigid_Body_Accelerator_Module, LEti::Module)

    private:
        glm::vec3 m_acceleration = {0.0f, 0.0f, 0.0f};
        bool m_rotate_with_object = false;

    private:
        bool m_enabled = true;

        LMD::Physics_Module__Rigid_Body* m_rigid_body = nullptr;

    public:
        inline void set_enabled(bool _value) { m_enabled = _value; }
        inline void set_acceleration(const glm::vec3& _value) { m_acceleration = _value; }
        inline void set_rotate_with_object(bool _value) { m_rotate_with_object = _value; }

        inline const glm::vec3& acceleration() const { return m_acceleration; }
        inline bool rotating_with_object() const { return m_rotate_with_object; }

    protected:
        void M_on_parent_object_set() override;

    public:
        void update(float _dt) override;

    };


    class Rigid_Body_Accelerator_Module_Stub : public LEti::Module_Stub
    {
    public:
        INIT_VARIABLE(LMD::Rigid_Body_Accelerator_Module_Stub, LEti::Module_Stub)

        INIT_FIELDS
        ADD_FIELD(bool, enabled)
        ADD_FIELD(glm::vec3, acceleration)
        ADD_FIELD(bool, rotate_with_object)
        FIELDS_END

    public:
        bool enabled = true;
        glm::vec3 acceleration = {0.0f, 0.0f, 0.0f};
        bool rotate_with_object = false;

    public:
        INIT_DEFAULT_BUILDER_STUB(Rigid_Body_Accelerator_Module)

        INIT_BUILDER_STUB_SETTERS
        ADD_BUILDER_STUB_SETTER(set_enabled, enabled)
        ADD_BUILDER_STUB_SETTER(set_acceleration, acceleration)
        ADD_BUILDER_STUB_SETTER(set_rotate_with_object, rotate_with_object)
        BUILDER_STUB_SETTERS_END

    };

}
