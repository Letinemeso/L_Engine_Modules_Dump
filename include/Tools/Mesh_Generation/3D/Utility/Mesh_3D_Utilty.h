#pragma once

#include <Data_Structures/Map.h>
#include <Data_Structures/Vector.h>
#include <Stuff/Quantized_Vector.h>
#include <Stuff/Signed_Coordinates.h>


namespace LMD
{
namespace Mesh_3D_Utility
{

    using Ids_Map = LDS::Map<LST::Quantized_Vector, unsigned int>;
    using Points_Vec = LDS::Vector<glm::vec3>;
    using IDs_Vec = LDS::Vector<unsigned int>;
    using Point_Neighbors_Vec = LDS::Vector<IDs_Vec>;

    struct Triangle
    {
        unsigned int id[3];
    };
    using Triangles_Vec = LDS::Vector<Triangle>;

    using Voxel_Triangles_Map = LDS::Map<LST::Signed_Coordinates, Triangles_Vec>;

}
}
