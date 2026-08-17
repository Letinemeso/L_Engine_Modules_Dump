#pragma once

#include <Stuff/Signed_Coordinates.h>
#include <Data_Structures/Map.h>

#include <Tools/Spatial_Grid/Spatial_Grid_2D.h>
#include <Tools/Voxel/Voxel_3D/Voxel_3D_Controller.h>
#include <Tools/Mesh_Generation/3D/Utility/Chunk_3D_Grid.h>
#include <Tools/Mesh_Generation/3D/Utility/Chunk_3D_Generation_Data.h>


namespace LMD
{

    class Chunk_3D_Layer
    {
    private:
        using Layer_Map = LDS::Map<LST::Signed_Coordinates, Chunk_3D_Generation_Data>;

        struct Layer_Data
        {
            Layer_Map chunks_data;
        };

    private:
        Layer_Data m_layer_prev;
        Layer_Data m_layer_curr;
        Layer_Data m_layer_next;

        unsigned int m_current_z_offset = 0;

        int m_z_min = 0;
        int m_z_max = 0;

    private:
        const Voxel_3D_Controller* m_voxel_controller = nullptr;
        unsigned int m_max_depth = 0;

    public:
        inline void set_voxel_controller(const Voxel_3D_Controller* _ptr) { m_voxel_controller = _ptr;  }
        inline void set_max_depth(unsigned int _value) { m_max_depth = _value; }

    private:
        void M_load_layer(Layer_Data& _layer_data, unsigned int _z);

    public:
        void clear();
        void reload();
        void load_next_layer();

    };

}
