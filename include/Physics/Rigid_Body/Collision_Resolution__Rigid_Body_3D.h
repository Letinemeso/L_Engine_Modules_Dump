#pragma once

#include <Collision_Resolution/Collision_Resolver.h>
#include <Collision_Resolution/Collision_Resolution__Physics_Module__Mesh.h>

#include <Physics/Rigid_Body/Physics_Module__Rigid_Body.h>


namespace LMD
{

    class Collision_Resolution__Rigid_Body_3D : public LPhys::Collision_Resolution_Interface
	{
    private:
        LPhys::Collision_Resolution__Physics_Module__Mesh m_default_collision_resolution;

        float m_soft_damping_multiplier = 0.75f;

        float m_soft_damping_min_velocity_squared = 0.01f;
        float m_soft_damping_min_angular_velocity_squared = 2.25f;

        float m_hard_damping_velocity_threshold_squared = 0.0001f;
        float m_hard_damping_angular_velocity_threshold_squared = 0.25f;

        float m_balancing_torque = 0.3f;

    public:
        inline LPhys::Collision_Resolution__Physics_Module__Mesh& default_collision_resolution() { return m_default_collision_resolution; }

        inline void set_soft_damping_multiplier(float _value) { m_soft_damping_multiplier = _value; }

        inline void set_soft_damping_min_velocity(float _value) { m_soft_damping_min_velocity_squared = _value * _value; }
        inline void set_soft_damping_min_angular_velocity(float _value) { m_soft_damping_min_angular_velocity_squared = _value * _value; }

        inline void set_hard_damping_velocity_threshold(float _value) { m_hard_damping_velocity_threshold_squared = _value * _value; }
        inline void set_hard_damping_angular_velocity_threshold(float _value) { m_hard_damping_angular_velocity_threshold_squared = _value * _value; }

    private:
        bool M_resolve_dynamic_vs_dynamic(const LPhys::Intersection_Data& _id, float _dt);
        bool M_resolve_dynamic_vs_static(const LPhys::Intersection_Data& _id, float _dt);

        void M_resolve_impulse(Physics_Module__Rigid_Body* _rb, const glm::vec3& _normal, const glm::vec3& _radius_vector, float _normal_velocity, float _mass_denominator);
        void M_resolve_contact(Physics_Module__Rigid_Body* _rb, const glm::vec3& _normal, const glm::vec3& _radius_vector, float _normal_velocity, float _mass_denominator);

        void M_apply_friction(Physics_Module__Rigid_Body* _rb, const glm::vec3& _normal, const glm::vec3& _radius_vector, float _normal_velocity);

        void M_damp_velocities(glm::vec3& _velocity, glm::vec3& _angular_velocity) const;

	public:
        bool resolve(const LPhys::Intersection_Data &_id, float _dt = 0.0f) override;

	};

}
