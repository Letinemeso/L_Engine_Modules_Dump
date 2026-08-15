#pragma once

#include <Tools/Voxel/Voxel_3D/Voxel_3D_Controller.h>


namespace LMD
{

    class Mesh_Extractor_3D
    {
    private:
        const Voxel_3D_Controller* m_voxel_controller = nullptr;

    private:
        using Points = LDS::Vector<glm::vec3>;
        using Points_Cache = LDS::Map<Vec_Storage, Point_Data>;
        using Used_Points_Grid = LMD::Spatial_Grid_3D<Points_Ids>;

    public:
        Mesh_Extractor_3D();
        ~Mesh_Extractor_3D();

    };

}
