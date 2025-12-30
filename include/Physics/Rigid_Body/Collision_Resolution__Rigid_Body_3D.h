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

        float m_impulse_ratio_after_collision = 0.75f;

        float m_min_damping_velocity = 0.1f;
        float m_min_damping_velocity_squared = m_min_damping_velocity * m_min_damping_velocity;
        float m_min_damping_angular_velocity = 1.5f;
        float m_min_damping_angular_velocity_squared = m_min_damping_angular_velocity * m_min_damping_angular_velocity;

        float m_hard_damping_velocity_threshold = 0.01f;
        float m_hard_damping_velocity_threshold_squared = m_hard_damping_velocity_threshold * m_hard_damping_velocity_threshold;
        float m_hard_damping_angular_velocity_threshold = 0.5f;
        float m_hard_damping_angular_velocity_threshold_squared = m_hard_damping_angular_velocity_threshold * m_hard_damping_angular_velocity_threshold;

        float m_balancing_torque = 0.3f;

    public:
        inline LPhys::Collision_Resolution__Physics_Module__Mesh& default_collision_resolution() { return m_default_collision_resolution; }

        inline void set_impulse_ratio_after_collision(float _value) { m_impulse_ratio_after_collision = _value; }

    private:
        bool M_resolve_dynamic_vs_dynamic(const LPhys::Intersection_Data& _id, float _dt);
        bool M_resolve_dynamic_vs_static(const LPhys::Intersection_Data& _id, float _dt);

	public:
        bool resolve(const LPhys::Intersection_Data &_id, float _dt = 0.0f) override;

	};

}
