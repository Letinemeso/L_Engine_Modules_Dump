#include <Tools/Voxel/Voxel_3D/Brushes/Voxel_3D_Brush.h>

using namespace LMD;


bool Voxel_3D_Brush::M_should_merge_subvoxel(const LMD::Voxel_3D* _voxel) const
{
    L_ASSERT(_voxel->is_split());

    for(unsigned int i = 0; i < Voxel_3D::Subvoxels_Amount; ++i)
    {
        if(_voxel->child(i)->is_split())
            return false;
    }

    for(unsigned int i = 0; i < Voxel_3D::Subvoxels_Amount - 1; ++i)
    {
        if(_voxel->child(i)->value() != _voxel->child(i + 1)->value())
            return false;
    }

    return true;
}



void Voxel_3D_Brush::M_apply_recursive(LMD::Voxel_3D* _voxel) const
{
    if(M_fully_inside(_voxel))
    {
        if(_voxel->is_split())
            _voxel->merge();

        _voxel->set_value(m_value);
        return;
    }

    if(M_partially_inside(_voxel))
    {
        if(!_voxel->is_split() && _voxel->depth() < m_max_depth)
            _voxel->split();

        if(!_voxel->is_split())
        {
            _voxel->set_value(m_value);
            return;
        }

        for(unsigned int i = 0; i < Voxel_3D::Subvoxels_Amount; ++i)
            M_apply_recursive(_voxel->child(i));

        return;
    }
}

void Voxel_3D_Brush::M_merge_excessive_subvoxels(LMD::Voxel_3D* _voxel) const
{
    if(!_voxel->is_split())
        return;

    for(unsigned int i = 0; i < Voxel_3D::Subvoxels_Amount; ++i)
        M_merge_excessive_subvoxels(_voxel->child(i));

    if(!M_should_merge_subvoxel(_voxel))
        return;

    unsigned int child_subvoxel_value = _voxel->child(0)->value();
    _voxel->merge();
    _voxel->set_value(child_subvoxel_value);
}



void Voxel_3D_Brush::apply(LMD::Voxel_3D* _voxel) const
{
    M_apply_recursive(_voxel);
    M_merge_excessive_subvoxels(_voxel);
}
