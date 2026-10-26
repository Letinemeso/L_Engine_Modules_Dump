#pragma once

#include <Stuff/Coordinates.h>
#include <Stuff/Signed_Coordinates.h>

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
        Chunk_3D_Grid(const Chunk_3D_Grid& _other);
        Chunk_3D_Grid(Chunk_3D_Grid&& _from);
        void operator=(const Chunk_3D_Grid& _other);
        void operator=(Chunk_3D_Grid&& _from);

    private:
        unsigned int M_calculate_subvoxel_value(const Voxel_3D& _subvoxel) const;
        void M_fill_subvoxel_area(const Voxel_3D& _subvoxel, const LST::Coordinates& _offset, unsigned int _area_size);

        LST::Coordinates M_convert_signed_coordinates(const LST::Signed_Coordinates& _coords) const;

    public:
        void construct_grid(const Voxel_3D& _from, unsigned int _max_depth, unsigned int _default_value);

        void fill_halo_left(const Chunk_3D_Grid& _neighbor);
        void fill_halo_right(const Chunk_3D_Grid& _neighbor);
        void fill_halo_bottom(const Chunk_3D_Grid& _neighbor);
        void fill_halo_top(const Chunk_3D_Grid& _neighbor);
        void fill_halo_back(const Chunk_3D_Grid& _neighbor);
        void fill_halo_front(const Chunk_3D_Grid& _neighbor);

    public:
        inline unsigned int grid_size() const { return m_grid.size_x() - 2; }
        inline unsigned int value_at(int _x, int _y, int _z) const { return m_grid.data(_x + 1, _y + 1, _z + 1); }
        inline unsigned int value_at(const LST::Signed_Coordinates& _coordinates) const { return m_grid.data( M_convert_signed_coordinates(_coordinates + LST::Signed_Coordinates(1, 1, 1)) ); }

    };

}
