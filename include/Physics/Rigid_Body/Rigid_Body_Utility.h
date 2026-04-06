#pragma once

#include <Physics/Rigid_Body/Physics_Module__Rigid_Body.h>


namespace LMD
{
namespace Rigid_Body_Utility
{

    glm::vec3 calculate_radius_vector(const Physics_Module__Rigid_Body& _rb, const glm::vec3& _point);

    glm::vec3 calculate_point_velocity(const Physics_Module__Rigid_Body& _rb, const glm::vec3& _radius_vector);

}
}
