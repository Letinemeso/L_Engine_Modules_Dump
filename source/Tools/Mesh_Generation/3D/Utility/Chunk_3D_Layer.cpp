#include <Tools/Mesh_Generation/3D/Utility/Chunk_3D_Layer.h>

#include <Stuff/Cast_Tools.h>

using namespace LMD;


void Chunk_3D_Layer::M_load_layer(Layer_Data& _layer_data, unsigned int _z)
{
    _layer_data.chunks_data.clear();

    for(Voxel_3D_Controller::Voxel_Map::Const_Iterator it = m_voxel_controller->voxels().iterator(); !it.end_reached(); ++it)
    {
        const LST::Signed_Coordinates& coords = it.key();

        if(coords.z() != _z)
            continue;

        Chunk_3D_Generation_Data generation_data(**it, m_max_depth);
        _layer_data.chunks_data.insert(coords, LST::move(generation_data));
    }
}



void Chunk_3D_Layer::clear()
{
    m_layer_prev.chunks_data.clear();
    m_layer_curr.chunks_data.clear();
    m_layer_next.chunks_data.clear();

    m_current_z_offset = 0;
}

void Chunk_3D_Layer::reload()
{
    L_ASSERT(m_voxel_controller);

    clear();

    Voxel_3D_Controller::Voxel_Coordinates_Range range = m_voxel_controller->calculate_voxel_coordinates_range();
    m_z_min = range.min.z();
    m_z_max = range.max.z();

    M_load_layer(m_layer_curr, m_z_min);
    M_load_layer(m_layer_next, m_z_max);
}

void Chunk_3D_Layer::load_next_layer()
{
    ++m_current_z_offset;

    m_layer_prev = LST::move(m_layer_curr);
    m_layer_curr = LST::move(m_layer_next);
    M_load_layer(m_layer_next, m_current_z_offset + 1);
}
