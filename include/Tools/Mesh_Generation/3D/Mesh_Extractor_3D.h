#pragma once

#include <Data_Structures/Map.h>
#include <Data_Structures/Vector.h>
#include <Stuff/Quantized_Vector.h>

#include <Variable_Base.h>
#include <Builder_Stub.h>

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


    class Mesh_Extractor_3D : public LV::Variable_Base
    {
    public:
        INIT_VARIABLE(LMD::Mesh_Extractor_3D, LV::Variable_Base)

    public:
        using Voxel_Meshes_Map = LDS::Map<LST::Signed_Coordinates, Mesh_3D>;

    private:
        Mesh_3D_Utility::Ids_Map m_ids_cache;
        Mesh_3D_Utility::Points_Vec m_points_cache;
        Mesh_3D_Utility::Voxel_Triangles_Map m_voxel_triangles;
        Voxel_Meshes_Map m_voxel_meshes_map;

        unsigned int m_max_extraction_depth = 0;
        float m_extraction_cell_size = 0.0f;

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


    class Mesh_Extractor_3D_Stub : public LV::Builder_Stub
    {
    public:
        INIT_VARIABLE(LMD::Mesh_Extractor_3D_Stub, LV::Builder_Stub)

        INIT_FIELDS
        ADD_FIELD(unsigned int, max_extraction_depth)
        FIELDS_END

        INIT_CHILDS
        ADD_CHILD("mesh_data_extractor__geometry", mesh_data_extractor__geometry)
        ADD_CHILD("mesh_data_extractor__texture", mesh_data_extractor__texture)
        ADD_CHILD("mesh_data_extractor__normals", mesh_data_extractor__normals)
        ADD_CHILD("mesh_smoother", mesh_smoother)
        CHILDS_END

    public:
        unsigned int max_extraction_depth = 0;

        Mesh_Data_Extractor_Stub* mesh_data_extractor__geometry = nullptr;
        Mesh_Data_Extractor_Stub* mesh_data_extractor__texture = nullptr;
        Mesh_Data_Extractor_Stub* mesh_data_extractor__normals = nullptr;

        Mesh_3D_Smoother_Stub* mesh_smoother = nullptr;

    public:
        INIT_DEFAULT_BUILDER_STUB(Mesh_Extractor_3D)

        INIT_BUILDER_STUB_SETTERS
        ADD_BUILDER_STUB_SETTER(set_max_extraction_depth, max_extraction_depth)
        ADD_BUILDER_STUB_SETTER(set_mesh_data_extractor__geometry, Mesh_Data_Extractor_Stub::construct_from_if_exists(mesh_data_extractor__geometry))
        ADD_BUILDER_STUB_SETTER(set_mesh_data_extractor__texture, Mesh_Data_Extractor_Stub::construct_from_if_exists(mesh_data_extractor__texture))
        ADD_BUILDER_STUB_SETTER(set_mesh_data_extractor__normals, Mesh_Data_Extractor_Stub::construct_from_if_exists(mesh_data_extractor__normals))
        ADD_BUILDER_STUB_SETTER(set_mesh_smoother, Mesh_3D_Smoother_Stub::construct_from_if_exists(mesh_smoother))
        BUILDER_STUB_SETTERS_END

    public:
        ~Mesh_Extractor_3D_Stub();

    };

}
