#include <Tools/Mesh_Generation/3D/Utility/Chunk_3D_Generation_Data.h>

#include <Stuff/Cast_Tools.h>

using namespace LMD;


Chunk_3D_Generation_Data::Chunk_3D_Generation_Data(const Voxel_3D& _voxel, unsigned int _max_depth)
    : m_voxel(_voxel)
{
    m_grid.construct_grid(_voxel, _max_depth);
}

Chunk_3D_Generation_Data::Chunk_3D_Generation_Data(Chunk_3D_Generation_Data&& _from)
    : m_voxel(_from.m_voxel)
{
    m_grid = LST::move(_from.m_grid);
}
