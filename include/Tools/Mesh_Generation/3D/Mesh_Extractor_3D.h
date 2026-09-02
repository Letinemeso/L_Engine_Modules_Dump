#pragma once

#include <Data_Structures/Map.h>
#include <Data_Structures/Vector.h>
#include <Stuff/Quantized_Vector.h>

#include <Tools/Voxel/Voxel_3D/Voxel_3D_Controller.h>


namespace LMD
{

    struct Mesh_3D
    {
        LDS::Vector<float> geometry;
        LDS::Vector<float> texture_coordinates;
        LDS::Vector<float> normals;
    };


    class Mesh_Extractor_3D
    {
    private:
        using Ids_Map = LDS::Map<LST::Quantized_Vector, unsigned int>;
        using Points_Vec = LDS::Vector<glm::vec3>;

        struct Triangle
        {
            unsigned int id[3];
        };
        using Triangles_Vec = LDS::Vector<Triangle>;

        using Voxel_Triangles_Map = LDS::Map<LST::Signed_Coordinates, Triangles_Vec>;
        using Voxel_Meshes_Map = LDS::Map<LST::Signed_Coordinates, Mesh_3D>;

    private:
        Ids_Map m_ids_cache;
        Points_Vec m_points_cache;
        Voxel_Triangles_Map m_voxel_triangles;
        Voxel_Meshes_Map m_voxel_meshes_map;

        unsigned int m_max_extraction_depth = 0;
        float m_extraction_cell_size = 0.0f;

    private:
        const Voxel_3D_Controller* m_voxel_controller = nullptr;

    public:
        Mesh_Extractor_3D();
        ~Mesh_Extractor_3D();

    public:
        inline void set_voxel_controller(const Voxel_3D_Controller* _ptr) { m_voxel_controller = _ptr; }
        inline void set_max_extraction_depth(unsigned int _value) { m_max_extraction_depth = _value; }

    private:
        unsigned int M_get_or_add_id(const glm::vec3& _vec);
        Triangle M_construct_triangle(const LDS::Vector<glm::vec3>& _raw_mesh, unsigned int _offset);

        void M_append_mesh_data(const LST::Signed_Coordinates& _coords, const LDS::Vector<glm::vec3>& _raw_mesh);

        void M_extract_geometry_data(LDS::Vector<float>& _geometry, const Triangles_Vec& _triangles);
        void M_extract_texture_coords_data(LDS::Vector<float>& _texture_coords, const Triangles_Vec& _triangles);
        void M_extract_normals_data(LDS::Vector<float>& _normals, const Triangles_Vec& _triangles);
        void M_extract_meshes();

    public:
        void clear();
        void extract();

    public:
        const Mesh_3D* get_mesh(const LST::Signed_Coordinates& _coords) const;

    };

}
