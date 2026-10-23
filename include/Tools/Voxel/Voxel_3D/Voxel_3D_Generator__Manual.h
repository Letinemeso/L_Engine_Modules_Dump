#pragma once

#include <Tools/Voxel/Voxel_3D/Voxel_3D_Generator.h>


namespace LMD
{

    class Voxel_3D_Generator__Manual : public Voxel_3D_Generator
    {
    public:
        INIT_VARIABLE(LMD::Voxel_3D_Generator__Manual, LV::Variable_Base)

    public:
        void generate() const override { };

    };

}
