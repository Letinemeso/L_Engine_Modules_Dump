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

        float m_impulse_ratio_after_collision = 0.99f;

        float m_min_damping_velocity = 0.01f;
        float m_min_damping_angular_velocity = 0.01f;

        float m_hard_damping_velocity_threshold = 0.0025f;
        float m_hard_damping_angular_velocity_threshold = 0.075f;

        float m_balancing_torque = 0.3f;

    public:
        inline LPhys::Collision_Resolution__Physics_Module__Mesh& default_collision_resolution() { return m_default_collision_resolution; }

        inline void set_impulse_ratio_after_collision(float _value) { m_impulse_ratio_after_collision = _value; }

    private:
        float M_calculate_kinetic_energy(const glm::vec3& _velocity, const glm::vec3& _angular_velocity, float _mass, float _moment_of_inertia) const;

        bool M_resolve_dynamic_vs_dynamic(const LPhys::Intersection_Data& _id, float _dt);
        bool M_resolve_dynamic_vs_static(const LPhys::Intersection_Data& _id, float _dt);

        void M_apply_torque(Physics_Module__Rigid_Body* _rb, glm::vec3& _angular_velocity, const glm::vec3& _point_of_contact, const glm::vec3& _normal, float _dt) const;

	public:
        bool resolve(const LPhys::Intersection_Data &_id, float _dt = 0.0f) override;

	};

}
