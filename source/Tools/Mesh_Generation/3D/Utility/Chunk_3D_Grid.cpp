#include <Tools/Mesh_Generation/3D/Utility/Chunk_3D_Grid.h>

#include <Stuff/Cast_Tools.h>
#include <Stuff/Math_Stuff.h>
#include <Data_Structures/Map.h>

using namespace LMD;


Chunk_3D_Grid::Chunk_3D_Grid()
{

}

Chunk_3D_Grid::Chunk_3D_Grid(const Chunk_3D_Grid& _other)
{
    m_max_depth = _other.m_max_depth;
    m_grid = _other.m_grid;
}

Chunk_3D_Grid::Chunk_3D_Grid(Chunk_3D_Grid&& _from)
{
    m_max_depth = _from.m_max_depth;
    m_grid = LST::move(_from.m_grid);
}

void Chunk_3D_Grid::operator=(const Chunk_3D_Grid& _other)
{
    m_max_depth = _other.m_max_depth;
    m_grid = _other.m_grid;
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

void Chunk_3D_Grid::M_fill_subvoxel_area(const Voxel_3D& _subvoxel, const LST::Coordinates& _offset, unsigned int _area_size)
{
    if(_subvoxel.is_split() && _subvoxel.depth() < m_max_depth)
    {
        unsigned int area_size_halved = _area_size / 2;
        L_ASSERT(_area_size > 0);

        M_fill_subvoxel_area(*_subvoxel.child(Voxel_3D::Subvoxel_Position::Left_Bottom_Close),
                             _offset, area_size_halved);
        M_fill_subvoxel_area(*_subvoxel.child(Voxel_3D::Subvoxel_Position::Right_Bottom_Close),
                             _offset + LST::Coordinates{area_size_halved, 0, 0}, area_size_halved);
        M_fill_subvoxel_area(*_subvoxel.child(Voxel_3D::Subvoxel_Position::Left_Top_Close),
                             _offset + LST::Coordinates{0, area_size_halved, 0}, area_size_halved);
        M_fill_subvoxel_area(*_subvoxel.child(Voxel_3D::Subvoxel_Position::Right_Top_Close),
                             _offset + LST::Coordinates{area_size_halved, area_size_halved, 0}, area_size_halved);
        M_fill_subvoxel_area(*_subvoxel.child(Voxel_3D::Subvoxel_Position::Left_Bottom_Far),
                             _offset + LST::Coordinates{0, 0, area_size_halved}, area_size_halved);
        M_fill_subvoxel_area(*_subvoxel.child(Voxel_3D::Subvoxel_Position::Right_Bottom_Far),
                             _offset + LST::Coordinates{area_size_halved, 0, area_size_halved}, area_size_halved);
        M_fill_subvoxel_area(*_subvoxel.child(Voxel_3D::Subvoxel_Position::Left_Top_Far),
                             _offset + LST::Coordinates{0, area_size_halved, area_size_halved}, area_size_halved);
        M_fill_subvoxel_area(*_subvoxel.child(Voxel_3D::Subvoxel_Position::Right_Top_Far),
                             _offset + LST::Coordinates{area_size_halved, area_size_halved, area_size_halved}, area_size_halved);

        return;
    }

    unsigned int id = M_calculate_subvoxel_value(_subvoxel);

    LST::Coordinates coords;
    LST::Coordinates limit = _offset + LST::Coordinates(_area_size, _area_size, _area_size);
    for(coords.z() = _offset.z(); coords.z() < limit.z(); ++coords.z())
    {
        for(coords.y() = _offset.y(); coords.y() < limit.y(); ++coords.y())
        {
            for(coords.x() = _offset.x(); coords.x() < limit.x(); ++coords.x())
            {
                m_grid.data(coords) = id;
            }
        }
    }
}


LST::Coordinates Chunk_3D_Grid::M_convert_signed_coordinates(const LST::Signed_Coordinates& _coords) const
{
    L_ASSERT(_coords.x() >= 0);
    L_ASSERT(_coords.y() >= 0);
    L_ASSERT(_coords.z() >= 0);

    LST::Coordinates result;
    for(unsigned int i = 0; i < 3; ++i)
        result[i] = (unsigned int)_coords[i];
    return result;
}



void Chunk_3D_Grid::construct_grid(const Voxel_3D& _from, unsigned int _max_depth)
{
    L_ASSERT(_from.depth() == 0);

    m_max_depth = _max_depth;

    unsigned int grid_size = LST::Math::pow_ui(2, _max_depth);
    m_grid.resize(grid_size + 2, grid_size + 2, grid_size + 2);

    M_fill_subvoxel_area(_from, {1, 1, 1}, grid_size);
}


void Chunk_3D_Grid::fill_halo_left(const Chunk_3D_Grid& _neighbor)
{
    unsigned int size = grid_size();
    L_ASSERT(_neighbor.grid_size() == size);
    unsigned int last_coord = size;

    for(unsigned int y = 0; y < size; ++y)
    {
        for(unsigned int z = 0; z < size; ++z)
        {
            m_grid.data(0, y + 1, z + 1) = _neighbor.value_at(last_coord, y, z);
        }
    }
}

void Chunk_3D_Grid::fill_halo_right(const Chunk_3D_Grid& _neighbor)
{
    unsigned int size = grid_size();
    L_ASSERT(_neighbor.grid_size() == size);
    unsigned int last_coord = size + 1;

    for(unsigned int y = 0; y < size; ++y)
    {
        for(unsigned int z = 0; z < size; ++z)
        {
            m_grid.data(last_coord, y + 1, z + 1) = _neighbor.value_at(0, y, z);
        }
    }
}

void Chunk_3D_Grid::fill_halo_bottom(const Chunk_3D_Grid& _neighbor)
{
    unsigned int size = grid_size();
    L_ASSERT(_neighbor.grid_size() == size);
    unsigned int last_coord = size;

    for(unsigned int x = 0; x < size; ++x)
    {
        for(unsigned int z = 0; z < size; ++z)
        {
            m_grid.data(x + 1, 0, z + 1) = _neighbor.value_at(x, last_coord, z);
        }
    }
}

void Chunk_3D_Grid::fill_halo_top(const Chunk_3D_Grid& _neighbor)
{
    unsigned int size = grid_size();
    L_ASSERT(_neighbor.grid_size() == size);
    unsigned int last_coord = size + 1;

    for(unsigned int x = 0; x < size; ++x)
    {
        for(unsigned int z = 0; z < size; ++z)
        {
            m_grid.data(x + 1, last_coord, z + 1) = _neighbor.value_at(x, 0, z);
        }
    }
}

void Chunk_3D_Grid::fill_halo_back(const Chunk_3D_Grid& _neighbor)
{
    unsigned int size = grid_size();
    L_ASSERT(_neighbor.grid_size() == size);
    unsigned int last_coord = size;

    for(unsigned int x = 0; x < size; ++x)
    {
        for(unsigned int y = 0; y < size; ++y)
        {
            m_grid.data(x + 1, y + 1, 0) = _neighbor.value_at(x, y, last_coord);
        }
    }
}

void Chunk_3D_Grid::fill_halo_front(const Chunk_3D_Grid& _neighbor)
{
    unsigned int size = grid_size();
    L_ASSERT(_neighbor.grid_size() == size);
    unsigned int last_coord = size + 1;

    for(unsigned int x = 0; x < size; ++x)
    {
        for(unsigned int y = 0; y < size; ++y)
        {
            m_grid.data(x + 1, y + 1, last_coord) = _neighbor.value_at(x, y, 0);
        }
    }
}


void Chunk_3D_Grid::fill_halo_left(unsigned int _value)
{
    unsigned int size = grid_size();

    for(unsigned int y = 0; y < size; ++y)
    {
        for(unsigned int z = 0; z < size; ++z)
        {
            m_grid.data(0, y + 1, z + 1) = _value;
        }
    }
}

void Chunk_3D_Grid::fill_halo_right(unsigned int _value)
{
    unsigned int size = grid_size();
    unsigned int last_coord = size + 1;

    for(unsigned int y = 0; y < size; ++y)
    {
        for(unsigned int z = 0; z < size; ++z)
        {
            m_grid.data(last_coord, y + 1, z + 1) = _value;
        }
    }
}

void Chunk_3D_Grid::fill_halo_bottom(unsigned int _value)
{
    unsigned int size = grid_size();

    for(unsigned int x = 0; x < size; ++x)
    {
        for(unsigned int z = 0; z < size; ++z)
        {
            m_grid.data(x + 1, 0, z + 1) = _value;
        }
    }
}

void Chunk_3D_Grid::fill_halo_top(unsigned int _value)
{
    unsigned int size = grid_size();
    unsigned int last_coord = size + 1;

    for(unsigned int x = 0; x < size; ++x)
    {
        for(unsigned int z = 0; z < size; ++z)
        {
            m_grid.data(x + 1, last_coord, z + 1) = _value;
        }
    }
}

void Chunk_3D_Grid::fill_halo_back(unsigned int _value)
{
    unsigned int size = grid_size();

    for(unsigned int x = 0; x < size; ++x)
    {
        for(unsigned int y = 0; y < size; ++y)
        {
            m_grid.data(x + 1, y + 1, 0) = _value;
        }
    }
}

void Chunk_3D_Grid::fill_halo_front(unsigned int _value)
{
    unsigned int size = grid_size();
    unsigned int last_coord = size + 1;

    for(unsigned int x = 0; x < size; ++x)
    {
        for(unsigned int y = 0; y < size; ++y)
        {
            m_grid.data(x + 1, y + 1, last_coord) = _value;
        }
    }
}
