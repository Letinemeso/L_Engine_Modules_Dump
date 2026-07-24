#pragma once

#include <Stuff/Function_Wrapper.h>

#include <Tools/Voxel/Voxel_2D/Voxel_2D.h>


namespace LMD
{

    using Voxel_Intersection_Check_Func = LST::Function<bool(Voxel_2D*)>;
    using Voxel_Modification_Func = LST::Function<bool(Voxel_2D*)>;
    using Voxel_Apply_Data_Func = LST::Function<void(Voxel_2D*)>;

    bool apply_data_to_voxel_recursive(Voxel_2D* _voxel, const Voxel_Intersection_Check_Func& _should_apply_to_whole, const Voxel_Intersection_Check_Func& _should_apply_partially, const Voxel_Apply_Data_Func& _apply_data);

}
