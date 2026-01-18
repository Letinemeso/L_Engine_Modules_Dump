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
        float m_restitution = 1.0f;
        float m_moment_of_inertia_scale = 1.0f;

        glm::vec3 m_velocity{0.0f, 0.0f, 0.0f};
        glm::vec3 m_angular_velocity = {0.0f, 0.0f, 0.0f};

        glm::mat3x3 m_inertia_tensor;
        glm::mat3x3 m_inertia_tensor_inverse;

    private:
        LPhys::Physical_Model* M_create_physical_model() const override;

    private:
        void M_debug_check_vec3(const glm::vec3& _vec) const;

    public:
        void set_mass(float _mass);
        void recalculate_raw_data();

        inline void set_restitution(float _value) { m_restitution = _value; }
        inline void set_moment_of_inertia_scale(float _value) { m_moment_of_inertia_scale = _value; }

        inline void set_velocity(const glm::vec3& _value) { L_DEBUG_FUNC_1ARG(M_debug_check_vec3, _value); m_velocity = _value; }
        inline void set_angular_velocity(const glm::vec3& _value) { L_DEBUG_FUNC_1ARG(M_debug_check_vec3, _value); m_angular_velocity = _value; }

        inline void apply_linear_impulse(const glm::vec3& _value) { L_DEBUG_FUNC_1ARG(M_debug_check_vec3, _value); m_velocity += _value; }
        inline void apply_rotation(const glm::vec3& _value) { L_DEBUG_FUNC_1ARG(M_debug_check_vec3, _value); m_angular_velocity += _value; }

    public:
        float mass() const;
        float mass_inverse() const;
        float moment_of_inertia() const;
        float moment_of_inertia_inverse() const;
        const glm::mat3x3& inertia_tensor() const;
        const glm::mat3x3& inertia_tensor_inverse() const;
        const glm::vec3& center_of_mass() const;

        inline Rigid_Body_Physical_Model* cast_physical_model() { return (Rigid_Body_Physical_Model*)get_physical_model(); }
        inline const Rigid_Body_Physical_Model* cast_physical_model() const { return (Rigid_Body_Physical_Model*)get_physical_model(); }

        inline float restitution() const { return m_restitution; }

        inline const glm::vec3& velocity() const { return m_velocity; }
        inline const glm::vec3& angular_velocity() const { return m_angular_velocity; }

    private:
        glm::vec3 M_calculate_world_position(const glm::vec3& _center_of_mass) const;
        glm::vec3 M_calculate_center_of_mass() const;

    public:
        void update(float _dt) override;

	};



    class Physics_Module_Stub__Rigid_Body : public LPhys::Physics_Module_Stub__Mesh
    {
    public:
        INIT_VARIABLE(LMD::Physics_Module_Stub__Rigid_Body, LPhys::Physics_Module_Stub__Mesh)

        INIT_FIELDS
        ADD_FIELD(float, mass)
        ADD_FIELD(float, restitution)
        ADD_FIELD(float, moment_of_inertia_scale)
        FIELDS_END

    public:
        float mass = 1.0f;
        float restitution = 1.0f;
        float moment_of_inertia_scale = 1.0f;

    protected:
        INIT_BUILDER_STUB(Physics_Module__Rigid_Body)
    };

}
