#include <Tools/Voxel/Voxel_2D/Voxel_2D_Modification_Tools.h>

using namespace LGL;


bool LGL::apply_data_to_voxel_recursive(Voxel_2D* _voxel, const Voxel_Intersection_Check_Func& _should_apply_to_whole, const Voxel_Intersection_Check_Func& _should_apply_partially, const Voxel_Apply_Data_Func& _apply_data)
{
    if(_should_apply_to_whole(_voxel))
    {
        if(_voxel->is_split())
            _voxel->merge();

        _apply_data(_voxel);
        return true;
    }

    if(!_should_apply_partially(_voxel))
        return false;

    if(_voxel->reached_max_depth())
    {
        _apply_data(_voxel);
        return true;
    }

    if(!_voxel->is_split())
        _voxel->split();

    bool result = false;
    for(unsigned int i=0; i<4; ++i)
    {
        if(apply_data_to_voxel_recursive(_voxel->child(i), _should_apply_to_whole, _should_apply_partially, _apply_data))
            result = true;
    }

    return result;
}
