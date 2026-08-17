#include <Tools/Mesh_Generation/3D/Utility/Chunk_3D_Grid.h>

#include <Stuff/Cast_Tools.h>
#include <Stuff/Math_Stuff.h>
#include <Data_Structures/Map.h>

using namespace LMD;


Chunk_3D_Grid::Chunk_3D_Grid()
{

}

Chunk_3D_Grid::Chunk_3D_Grid(Chunk_3D_Grid&& _from)
{
    m_max_depth = _from.m_max_depth;
    m_grid = LST::move(_from.m_grid);
}

void Chunk_3D_Grid::operator=(Chunk_3D_Grid&& _from)
{
    m_max_depth = _from.m_max_depth;
    m_grid = LST::move(_from.m_grid);
}



unsigned int Chunk_3D_Grid::M_calculate_subvoxel_value(const Voxel_3D& _subvoxel) const
{
    if(!_subvoxel.is_split())
        return _subvoxel.value();

    using Counter_Map = LDS::Map<unsigned int, unsigned int>;
    Counter_Map subvalues_counter;
    for(unsigned int i = 0; i < Voxel_3D::Subvoxels_Amount; ++i)
    {
        unsigned int subvalue = M_calculate_subvoxel_value(*_subvoxel.child(i));

        Counter_Map::Iterator it = subvalues_counter.find_or_insert(subvalue, 0);
        ++*it;
    }

    unsigned int most_common_value = 0;
    unsigned int most_common_value_amount = 0;
    for(Counter_Map::Iterator it = subvalues_counter.iterator(); !it.end_reached(); ++it)
    {
        if(*it <= most_common_value_amount)
            continue;

        most_common_value = it.key();
        most_common_value_amount = *it;
    }

    return most_common_value;
}

void Chunk_3D_Grid::M_fill_subvoxel_area(const Voxel_3D& _subvoxel, const Spatial_Grid_3D_Indices& _offset, unsigned int _area_size)
{
    if(_subvoxel.is_split() && _subvoxel.depth() < m_max_depth)
    {
        unsigned int area_size_halved = _area_size / 2;
        L_ASSERT(_area_size > 0);

        M_fill_subvoxel_area(*_subvoxel.child(Voxel_3D::Subvoxel_Position::Left_Bottom_Close),
                             _offset, area_size_halved);
        M_fill_subvoxel_area(*_subvoxel.child(Voxel_3D::Subvoxel_Position::Right_Bottom_Close),
                             _offset + Spatial_Grid_3D_Indices{area_size_halved, 0, 0}, area_size_halved);
        M_fill_subvoxel_area(*_subvoxel.child(Voxel_3D::Subvoxel_Position::Left_Top_Close),
                             _offset + Spatial_Grid_3D_Indices{0, area_size_halved, 0}, area_size_halved);
        M_fill_subvoxel_area(*_subvoxel.child(Voxel_3D::Subvoxel_Position::Right_Top_Close),
                             _offset + Spatial_Grid_3D_Indices{area_size_halved, area_size_halved, 0}, area_size_halved);
        M_fill_subvoxel_area(*_subvoxel.child(Voxel_3D::Subvoxel_Position::Left_Bottom_Far),
                             _offset + Spatial_Grid_3D_Indices{0, 0, area_size_halved}, area_size_halved);
        M_fill_subvoxel_area(*_subvoxel.child(Voxel_3D::Subvoxel_Position::Right_Bottom_Far),
                             _offset + Spatial_Grid_3D_Indices{_area_size, 0, area_size_halved}, area_size_halved);
        M_fill_subvoxel_area(*_subvoxel.child(Voxel_3D::Subvoxel_Position::Left_Top_Far),
                             _offset + Spatial_Grid_3D_Indices{0, area_size_halved, area_size_halved}, area_size_halved);
        M_fill_subvoxel_area(*_subvoxel.child(Voxel_3D::Subvoxel_Position::Right_Top_Far),
                             _offset + Spatial_Grid_3D_Indices{_area_size, area_size_halved, area_size_halved}, area_size_halved);

        return;
    }

    unsigned int id = M_calculate_subvoxel_value(_subvoxel);

    for(unsigned int x = _offset.x; x < _offset.x + _area_size; ++x)
    {
        for(unsigned int y = _offset.y; y < _offset.y + _area_size; ++y)
        {
            for(unsigned int z = _offset.z; z < _offset.z + _area_size; ++z)
            {
                m_grid.data(x, y, z) = id;
            }
        }
    }
}



void Chunk_3D_Grid::construct_grid(const Voxel_3D& _from, unsigned int _max_depth)
{
    L_ASSERT(_from.depth() == 0);

    m_max_depth = _max_depth;

    unsigned int grid_size = LST::Math::pow_ui(2, _max_depth);
    m_grid.resize(grid_size, grid_size, grid_size);

    M_fill_subvoxel_area(_from, {0, 0, 0}, grid_size);
}
