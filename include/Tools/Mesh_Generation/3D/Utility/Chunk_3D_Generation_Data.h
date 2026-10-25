#pragma once

#include <Data_Structures/Vector.h>
#include <Stuff/Coordinates.h>

#include <Tools/Voxel/Voxel_3D/Voxel_3D.h>
#include <Tools/Mesh_Generation/3D/Utility/Chunk_3D_Grid.h>


namespace LMD
{

    class Chunk_3D_Generation_Data
    {
    public:
        struct Neighbors_Data
        {
            const Chunk_3D_Generation_Data* data[3][3][3] = {{{nullptr}}};
        };
        struct Neighboring_Cells_Data
        {
            unsigned int values[3][3][3] = {{{0}}};
        };
        struct Neighbor_Cell_Location
        {
            const Chunk_3D_Generation_Data* chunk = nullptr;
            LST::Coordinates coordinates;
        };

    private:
        const Voxel_3D* m_voxel = nullptr;

        Chunk_3D_Grid m_grid;
        LDS::Vector<glm::vec3> m_points;

        float m_cell_size = 0.0f;
        float m_cell_size_halved = 0.0f;
        unsigned int m_default_value = 0;

    public:
        Chunk_3D_Generation_Data(const Voxel_3D* _voxel, unsigned int _max_depth, unsigned int _default_value);
        Chunk_3D_Generation_Data(const Chunk_3D_Generation_Data& _other);
        Chunk_3D_Generation_Data(Chunk_3D_Generation_Data&& _from);
        void operator=(const Chunk_3D_Generation_Data& _other);
        void operator=(Chunk_3D_Generation_Data&& _from);

    public:
        inline const Chunk_3D_Grid& grid() const { return m_grid; }
        inline const LDS::Vector<glm::vec3>& points() const { return m_points; }

    private:
        void M_append_quad(const glm::vec3& _point_0, const glm::vec3& _point_1, const glm::vec3& _point_2, const glm::vec3& _point_3);
        void M_append_cell_data(const Neighbors_Data& _neighbors, const LST::Coordinates& _coords);

        void M_fill_grid_halo(const Neighbors_Data& _neighbors);

    public:
        void extract_points(const Neighbors_Data& _neighbors);

    };

}
