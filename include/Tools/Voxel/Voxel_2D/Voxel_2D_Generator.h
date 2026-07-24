#pragma once

#include <Tools/Voxel/Voxel_2D/Voxel_2D.h>


namespace LGL
{

    class Voxel_2D_Generator
    {
    public:
        Voxel_2D_Generator() { }
        virtual ~Voxel_2D_Generator() { }

    public:
        virtual Voxel_2D* generate(int _index_x, int _index_y, unsigned int _max_depth, float _size_x, float _size_y) const = 0;

    };

}
