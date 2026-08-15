#pragma once

#include <Data_Structures/Map.h>
#include <Stuff/Signed_Coordinates.h>

#include <Tools/Voxel/Voxel_3D/Voxel_3D.h>


namespace LMD
{

    class Voxel_3D_Controller
    {
    private:
        using Voxel_Map = LDS::Map<LST::Signed_Coordinates, Voxel_3D*>;
        Voxel_Map m_voxels;

        float m_voxel_size = 0.0f;
        float m_voxel_size_inverted = 0.0f;

    public:
        Voxel_3D_Controller(float _voxel_size);
        Voxel_3D_Controller(const Voxel_3D_Controller& _other);
        Voxel_3D_Controller(Voxel_3D_Controller&& _from);
        void operator=(const Voxel_3D_Controller& _other);
        void operator=(Voxel_3D_Controller&& _from);

        ~Voxel_3D_Controller();

    public:
        void clear();

    public:
        LST::Signed_Coordinates calculate_coordinates(const glm::vec3& _at) const;

        Voxel_3D* get_voxel(const LST::Signed_Coordinates& _coordinates) const;
        Voxel_3D* get_voxel(const glm::vec3& _at) const;
        Voxel_3D* get_or_insert_voxel(const LST::Signed_Coordinates& _coordinates);
        Voxel_3D* get_or_insert_voxel(const glm::vec3& _at);

    };

}
