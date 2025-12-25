#pragma once

#include <Modules/Physics_Module__Mesh.h>

#include <Physics/Rigid_Body/Rigid_Body_Physical_Model.h>


namespace LMD
{

    class Physics_Module__Rigid_Body : public LPhys::Physics_Module__Mesh
	{
    public:
        INIT_VARIABLE(LMD::Physics_Module__Rigid_Body, LPhys::Physics_Module__Mesh)

    private:
        glm::vec3 m_velocity{0.0f, 0.0f, 0.0f};
        float m_angular_velocity = 0.0f;

    private:
        LPhys::Physical_Model* M_create_physical_model() const override;

    public:
        void set_mass(float _mass);

        inline void set_velocity(const glm::vec3& _v) { m_velocity = _v; }
        inline void set_angular_velocity(float _av) { m_angular_velocity = _av; }

        inline void apply_linear_impulse(const glm::vec3& _imp) { m_velocity += _imp; }
        inline void apply_rotation(float _av) { m_angular_velocity += _av; }

    public:
        float mass() const;
        float moment_of_inertia() const;
        const glm::vec3& center_of_mass() const;

        inline const glm::vec3& velocity() const { return m_velocity; }
        inline float angular_velocity() const { return m_angular_velocity; }

    public:
        void update(float _dt) override;

	};



    class Physics_Module_Stub__Rigid_Body : public LPhys::Physics_Module_Stub__Mesh
    {
    public:
        INIT_VARIABLE(LMD::Physics_Module_Stub__Rigid_Body, LPhys::Physics_Module_Stub__Mesh)

        INIT_FIELDS
        ADD_FIELD(float, mass)
        FIELDS_END

    public:
        float mass = 1.0f;

    protected:
        INIT_BUILDER_STUB(Physics_Module__Rigid_Body)
    };

}
