#include <Tools/Voxel/Voxel_3D/Voxel_3D_Generator.h>

using namespace LMD;


void Voxel_3D_Generator::M_apply_brush(const Voxel_3D_Brush& _brush) const
{
    L_ASSERT(m_voxel_controller);

    Voxel_3D_Brush::Border brush_border = _brush.calculate_borders();

    LST::Signed_Coordinates coords_offset = m_voxel_controller->calculate_coordinates(brush_border.offset);
    LST::Signed_Coordinates coords_limit = m_voxel_controller->calculate_coordinates(brush_border.limit);

    for(int x = coords_offset.x(); x <= coords_limit.x(); ++x)
    {
        for(int y = coords_offset.y(); y <= coords_limit.y(); ++y)
        {
            for(int z = coords_offset.z(); z <= coords_limit.z(); ++z)
            {
                LST::Signed_Coordinates coords = {x, y, z};
                Voxel_3D* voxel = m_voxel_controller->get_or_insert_voxel(coords);
                _brush.apply(voxel);
            }
        }
    }
}
