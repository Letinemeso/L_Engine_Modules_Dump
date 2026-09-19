#pragma once

#include <Tools/Mesh_Generation/3D/Utility/Mesh_3D_Utilty.h>


namespace LMD
{

class Mesh_3D_Smoother
{
public:
    virtual ~Mesh_3D_Smoother() { }

public:
    virtual Mesh_3D_Utility::Points_Vec smooth(const Mesh_3D_Utility::Points_Vec& _old_mesh,
                                               const Mesh_3D_Utility::Voxel_Triangles_Map& _mesh_structure) const = 0;

};

}
