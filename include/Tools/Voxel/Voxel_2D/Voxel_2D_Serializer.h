#pragma once

#include <Stuff/Function_Wrapper.h>

#include <Tools/Voxel/Voxel_2D/Voxel_2D.h>


namespace LGL
{

    class Voxel_2D_Serializer
    {
    public:
        virtual ~Voxel_2D_Serializer() { }

    public:
        virtual void save_voxel(const Voxel_2D* _voxel, int _index_x, int _index_y) const = 0;
        virtual Voxel_2D* load_voxel(int _index_x, int _index_y) const = 0;

    };

}
