#pragma once

#include <Tools/Voxel/Voxel_3D/Voxel_3D_Controller.h>


namespace LMD
{

    class Mesh_Extractor_3D
    {
    private:
        const Voxel_3D_Controller* m_voxel_controller = nullptr;

    private:

    public:
        Mesh_Extractor_3D();
        ~Mesh_Extractor_3D();

    };

}
