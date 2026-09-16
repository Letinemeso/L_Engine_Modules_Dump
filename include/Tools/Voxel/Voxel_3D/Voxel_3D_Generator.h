#pragma once

#include <Tools/Voxel/Voxel_3D/Voxel_3D_Controller.h>
#include <Tools/Voxel/Voxel_3D/Brushes/Voxel_3D_Brush.h>


namespace LMD
{

    class Voxel_3D_Generator
    {
    private:
        Voxel_3D_Controller* m_voxel_controller = nullptr;

        unsigned int m_max_voxel_depth = 0;

    public:
        Voxel_3D_Generator(Voxel_3D_Controller* _voxel_controller = nullptr) { m_voxel_controller = _voxel_controller; }

    public:
        inline void set_voxel_controller(Voxel_3D_Controller* _ptr) { m_voxel_controller = _ptr; }
        inline void set_max_voxel_depth(unsigned int _value) { m_max_voxel_depth = _value; }

        inline Voxel_3D_Controller* voxel_controller() const { return m_voxel_controller; }
        inline unsigned int max_voxel_depth() const { return m_max_voxel_depth; }

    protected:
        void M_apply_brush(const Voxel_3D_Brush& _brush) const;

    public:
        virtual void generate() = 0;

    };

}
