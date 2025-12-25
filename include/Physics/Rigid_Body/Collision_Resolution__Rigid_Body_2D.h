#pragma once

#include <Collision_Resolution/Collision_Resolver.h>


namespace LMD
{

    class Collision_Resolution__Rigid_Body_2D : public LPhys::Collision_Resolution_Interface        //  this is a pretty old stuff and probably works bad
	{
    private:
        float M_calculate_kinetic_energy(const glm::vec3& _velocity, float _angular_velocity, float _mass, float _moment_of_inertia) const;

	public:
        bool resolve(const LPhys::Intersection_Data &_id, float _dt = 0.0f) override;

	};

}
