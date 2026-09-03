#pragma once

#include <Tools/Voxel/Voxel_3D/Voxel_3D.h>


namespace LMD
{

    class Voxel_3D_Brush
    {
    private:
        unsigned int m_max_depth = 0;
        unsigned int m_value = 0;

    public:
        inline void set_max_depth(unsigned int _value) { m_max_depth = _value; }
        inline void set_value(unsigned int _value) { m_value = _value; }

    protected:
        virtual bool M_fully_inside(const LMD::Voxel_3D* _voxel) const = 0;
        virtual bool M_partially_inside(const LMD::Voxel_3D* _voxel) const = 0;

    private:
        bool M_should_merge_subvoxel(const LMD::Voxel_3D* _voxel) const;

        void M_apply_recursive(LMD::Voxel_3D* _voxel) const;
        void M_merge_excessive_subvoxels(LMD::Voxel_3D* _voxel) const;

    public:
        void apply(LMD::Voxel_3D* _voxel) const;

    };

}
