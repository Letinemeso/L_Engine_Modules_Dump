#include <Tools/Mesh_Generation/3D/Utility/Chunk_3D_Layer.h>

#include <Stuff/Cast_Tools.h>

using namespace LMD;


Chunk_3D_Generation_Data::Neighbors_Data Chunk_3D_Layer::M_find_neighbors(const LST::Signed_Coordinates& _coords) const
{
    Chunk_3D_Generation_Data::Neighbors_Data result;

    for(int x = -1; x < 2; ++x)
    {
        for(int y = -1; y < 2; ++y)
        {
            LST::Signed_Coordinates coords_prev = _coords + LST::Signed_Coordinates(x, y, -1);
            LST::Signed_Coordinates coords_curr = _coords + LST::Signed_Coordinates(x, y, 0);
            LST::Signed_Coordinates coords_next = _coords + LST::Signed_Coordinates(x, y, 1);

            Layer_Map::Const_Iterator maybe_it_prev = m_layer_prev.find(coords_prev);
            Layer_Map::Const_Iterator maybe_it_curr = m_layer_prev.find(coords_curr);
            Layer_Map::Const_Iterator maybe_it_next = m_layer_prev.find(coords_next);

            if(maybe_it_prev.is_ok())
                result.data[x][y][0] = &(*maybe_it_prev);
            if(maybe_it_curr.is_ok())
                result.data[x][y][1] = &(*maybe_it_curr);
            if(maybe_it_next.is_ok())
                result.data[x][y][2] = &(*maybe_it_next);
        }
    }

    return result;
}


void Chunk_3D_Layer::M_load_layer(Layer_Map& _layer_data, unsigned int _z)
{
    _layer_data.clear();

    for(Voxel_3D_Controller::Voxel_Map::Const_Iterator it = m_voxel_controller->voxels().iterator(); !it.end_reached(); ++it)
    {
        const LST::Signed_Coordinates& coords = it.key();

        if(coords.z() != _z)
            continue;

        Chunk_3D_Generation_Data generation_data(*it, m_max_depth);
        _layer_data.insert(coords, LST::move(generation_data));
    }

    for(Layer_Map::Iterator it = _layer_data.iterator(); !it.end_reached(); ++it)
    {
        const LST::Signed_Coordinates& coords = it.key();
        Chunk_3D_Generation_Data& generation_data = *it;

        Chunk_3D_Generation_Data::Neighbors_Data neighbors = M_find_neighbors(coords);

        generation_data.extract_points(neighbors);
    }
}



void Chunk_3D_Layer::clear()
{
    m_layer_prev.clear();
    m_layer_curr.clear();
    m_layer_next.clear();

    m_current_z_offset = 0;
}

void Chunk_3D_Layer::reload()
{
    L_ASSERT(m_voxel_controller);

    clear();

    Voxel_3D_Controller::Voxel_Coordinates_Range range = m_voxel_controller->calculate_voxel_coordinates_range();
    m_z_min = range.min.z();
    m_z_max = range.max.z();

    m_current_z_offset = m_z_min;

    M_load_layer(m_layer_curr, m_z_min);
    M_load_layer(m_layer_next, m_z_min + 1);
}

void Chunk_3D_Layer::load_next_layer()
{
    ++m_current_z_offset;

    m_layer_prev = LST::move(m_layer_curr);
    m_layer_curr = LST::move(m_layer_next);
    M_load_layer(m_layer_next, m_current_z_offset + 1);
}
