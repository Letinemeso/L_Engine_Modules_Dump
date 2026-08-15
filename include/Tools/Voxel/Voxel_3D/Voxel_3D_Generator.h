#pragma once

#include <Tools/Voxel/Voxel_3D/Voxel_3D_Controller.h>


namespace LMD
{

    class Voxel_3D_Generator
    {
    private:
        Voxel_3D_Controller* m_voxel_controller = nullptr;

    public:
        Voxel_3D_Generator(Voxel_3D_Controller* _voxel_controller = nullptr) { m_voxel_controller = _voxel_controller; }

    public:
        inline void set_voxel_controller(Voxel_3D_Controller* _ptr) { m_voxel_controller = _ptr; }
        inline Voxel_3D_Controller* voxel_controller() const { return m_voxel_controller; }

    public:
        virtual void generate() = 0;

    };

}
