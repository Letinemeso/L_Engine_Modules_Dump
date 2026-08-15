#include <Tools/Mesh_Generation/3D/Mesh_Generator_3D.h>

using namespace LMD;


Mesh_Generator_3D::Mesh_Generator_3D(float _voxel_size, unsigned int _max_voxel_depth)
    : m_voxel_controller(_voxel_size), m_max_voxel_depth(_max_voxel_depth)
{

}

Mesh_Generator_3D::~Mesh_Generator_3D()
{

}
