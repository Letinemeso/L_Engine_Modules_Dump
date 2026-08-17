#pragma once

#include <Tools/Spatial_Grid/Spatial_Grid_3D.h>
#include <Tools/Voxel/Voxel_3D/Voxel_3D.h>


namespace LMD
{

    class Chunk_3D_Grid
    {
    private:
        unsigned int m_max_depth = 0;
        LMD::Spatial_Grid_3D<unsigned int> m_grid;

    public:
        Chunk_3D_Grid();
        Chunk_3D_Grid(Chunk_3D_Grid&& _from);
        void operator=(Chunk_3D_Grid&& _from);

    private:
        unsigned int M_calculate_subvoxel_value(const Voxel_3D& _subvoxel) const;
        void M_fill_subvoxel_area(const Voxel_3D& _subvoxel, const Spatial_Grid_3D_Indices& _offset, unsigned int _area_size);

    public:
        void construct_grid(const Voxel_3D& _from, unsigned int _max_depth);

    public:
        inline unsigned int grid_size() const { return m_grid.size_x(); }
        inline unsigned int value_at(unsigned int _x, unsigned int _y, unsigned int _z) const { return m_grid.data(_x, _y, _z); }

    };

}
