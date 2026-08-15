#pragma once

#include <Tools/Voxel/Voxel_3D/Voxel_3D_Controller.h>


namespace LMD
{

    class Mesh_Generator_3D
    {
    private:
        Voxel_3D_Controller m_voxel_controller;

        unsigned int m_max_voxel_depth = 0;

    public:
        Mesh_Generator_3D(float _voxel_size, unsigned int _max_voxel_depth);
        ~Mesh_Generator_3D();

    public:
        virtual void generate() = 0;

    };

}
