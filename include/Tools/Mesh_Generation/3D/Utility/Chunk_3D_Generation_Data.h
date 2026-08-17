#pragma once

#include <Tools/Voxel/Voxel_3D/Voxel_3D.h>
#include <Tools/Mesh_Generation/3D/Utility/Chunk_3D_Grid.h>


namespace LMD
{

    class Chunk_3D_Generation_Data
    {
    private:
        const Voxel_3D& m_voxel;

        Chunk_3D_Grid m_grid;

    public:
        Chunk_3D_Generation_Data(const Voxel_3D& _voxel, unsigned int _max_depth);
        Chunk_3D_Generation_Data(Chunk_3D_Generation_Data&& _from);

    public:
        inline const Chunk_3D_Grid& grid() const { return m_grid; }

    };

}
