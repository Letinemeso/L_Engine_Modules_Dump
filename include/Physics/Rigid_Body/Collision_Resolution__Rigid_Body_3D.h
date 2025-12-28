#pragma once

#include <Collision_Resolution/Collision_Resolver.h>
#include <Collision_Resolution/Collision_Resolution__Physics_Module__Mesh.h>

#include <Physics/Rigid_Body/Physics_Module__Rigid_Body.h>


namespace LMD
{

    class Collision_Resolution__Rigid_Body_3D : public LPhys::Collision_Resolution_Interface        //  this is a pretty old stuff and probably works bad
	{
    private:
        LPhys::Collision_Resolution__Physics_Module__Mesh m_default_collision_resolution;

    public:
        inline LPhys::Collision_Resolution__Physics_Module__Mesh& default_collision_resolution() { return m_default_collision_resolution; }

    private:
        float M_calculate_kinetic_energy(const glm::vec3& _velocity, const glm::vec3& _angular_velocity, float _mass, float _moment_of_inertia) const;

        bool M_resolve_dynamic_vs_dynamic(const LPhys::Intersection_Data& _id, float _dt);
        bool M_resolve_dynamic_vs_static(const LPhys::Intersection_Data& _id, float _dt);

	public:
        bool resolve(const LPhys::Intersection_Data &_id, float _dt = 0.0f) override;

	};

}
