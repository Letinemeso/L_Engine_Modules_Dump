#pragma once

#include <Data_Structures/Map.h>
#include <Data_Structures/Vector.h>
#include <Stuff/Quantized_Vector.h>

#include <Tools/Mesh_Generation/3D/Utility/Mesh_3D_Utilty.h>
#include <Tools/Mesh_Generation/3D/Mesh_Data_Extractors/Mesh_Data_Extractor.h>
#include <Tools/Mesh_Generation/3D/Mesh_Smoothers/Mesh_3D_Smoother.h>
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
    public:
        using Voxel_Meshes_Map = LDS::Map<LST::Signed_Coordinates, Mesh_3D>;

    private:
        Mesh_3D_Utility::Ids_Map m_ids_cache;
        Mesh_3D_Utility::Points_Vec m_points_cache;
        Mesh_3D_Utility::Voxel_Triangles_Map m_voxel_triangles;
        Voxel_Meshes_Map m_voxel_meshes_map;

        unsigned int m_max_extraction_depth = 0;
        float m_extraction_cell_size = 0.0f;

        float m_smooth_factor = 1.0f;

        Mesh_Data_Extractor* m_mesh_data_extractor__geometry = nullptr;
        Mesh_Data_Extractor* m_mesh_data_extractor__texture = nullptr;
        Mesh_Data_Extractor* m_mesh_data_extractor__normals = nullptr;

        Mesh_3D_Smoother* m_mesh_smoother = nullptr;

    private:
        const Voxel_3D_Controller* m_voxel_controller = nullptr;

    public:
        Mesh_Extractor_3D();
        ~Mesh_Extractor_3D();

    public:
        inline void set_voxel_controller(const Voxel_3D_Controller* _ptr) { m_voxel_controller = _ptr; }
        inline void set_max_extraction_depth(unsigned int _value) { m_max_extraction_depth = _value; }

        inline void set_smooth_factor(float _value) { m_smooth_factor = _value; }

        inline void set_mesh_data_extractor__geometry(Mesh_Data_Extractor* _ptr) { delete m_mesh_data_extractor__geometry; m_mesh_data_extractor__geometry = _ptr; }
        inline void set_mesh_data_extractor__texture(Mesh_Data_Extractor* _ptr) { delete m_mesh_data_extractor__texture; m_mesh_data_extractor__texture = _ptr; }
        inline void set_mesh_data_extractor__normals(Mesh_Data_Extractor* _ptr) { delete m_mesh_data_extractor__normals; m_mesh_data_extractor__normals = _ptr; }

        inline void set_mesh_smoother(Mesh_3D_Smoother* _ptr) { delete m_mesh_smoother; m_mesh_smoother = _ptr; }

        inline const Voxel_Meshes_Map& get_meshes() const { return m_voxel_meshes_map; }

    public:
        void set_default_mesh_data_extractors();

    private:
        unsigned int M_get_or_add_id(const glm::vec3& _vec);
        Mesh_3D_Utility::Triangle M_construct_triangle(const LDS::Vector<glm::vec3>& _raw_mesh, unsigned int _offset);

        void M_append_mesh_data(const LST::Signed_Coordinates& _coords, const LDS::Vector<glm::vec3>& _raw_mesh);
        void M_extract_meshes_data();

        void M_smooth_points();

        void M_extract_meshes();

    public:
        void clear();
        void extract();

    public:
        const Mesh_3D* get_mesh(const LST::Signed_Coordinates& _coords) const;
        Mesh_3D construct_combined_mesh() const;

    };

}
