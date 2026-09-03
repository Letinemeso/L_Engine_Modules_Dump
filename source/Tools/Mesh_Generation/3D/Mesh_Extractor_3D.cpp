#include <Tools/Mesh_Generation/3D/Mesh_Extractor_3D.h>

#include <Stuff/Math_Stuff.h>

#include <Tools/Mesh_Generation/3D/Utility/Chunk_3D_Layer.h>

using namespace LMD;


Mesh_Extractor_3D::Mesh_Extractor_3D()
{

}

Mesh_Extractor_3D::~Mesh_Extractor_3D()
{

}



unsigned int Mesh_Extractor_3D::M_get_or_add_id(const glm::vec3& _vec)
{
    LST::Quantized_Vector quantized_vector(_vec);

    Ids_Map::Iterator maybe_id_it = m_ids_cache.find(quantized_vector);
    if(maybe_id_it.is_ok())
        return *maybe_id_it;

    unsigned int new_id = m_points_cache.size();
    m_ids_cache.insert(quantized_vector, new_id);
    m_points_cache.push(_vec);

    return new_id;
}

Mesh_Extractor_3D::Triangle Mesh_Extractor_3D::M_construct_triangle(const LDS::Vector<glm::vec3>& _raw_mesh, unsigned int _offset)
{
    Triangle result;

    for(unsigned int i = 0; i < 3; ++i)
    {
        const glm::vec3& vec = _raw_mesh[_offset + i];
        unsigned int id = M_get_or_add_id(vec);

        result.id[i] = id;
    }

    return result;
}


void Mesh_Extractor_3D::M_append_mesh_data(const LST::Signed_Coordinates& _coords, const LDS::Vector<glm::vec3>& _raw_mesh)
{
    L_ASSERT(_raw_mesh.size() > 0);
    L_ASSERT(_raw_mesh.size() % 3 == 0);
    L_ASSERT(!m_voxel_triangles.find(_coords).is_ok());

    Triangles_Vec triangles(_raw_mesh.size() / 3);
    for(unsigned int i = 0; i < _raw_mesh.size(); i += 3)
        triangles.push( M_construct_triangle(_raw_mesh, i) );

    m_voxel_triangles.insert(_coords, LST::move(triangles));
}


void Mesh_Extractor_3D::M_extract_geometry_data(LDS::Vector<float>& _geometry, const Triangles_Vec& _triangles)
{
    _geometry.clear();
    _geometry.resize(_triangles.size() * 9);

    for(unsigned int t_i = 0; t_i < _triangles.size(); ++t_i)
    {
        const Triangle& triangle = _triangles[t_i];
        for(unsigned int i = 0; i < 3; ++i)
        {
            const glm::vec3& vec = m_points_cache[ triangle.id[i] ];

            for(unsigned int v_i = 0; v_i < 3; ++v_i)
                _geometry.push( vec[v_i] );
        }
    }
}

void Mesh_Extractor_3D::M_extract_texture_coords_data(LDS::Vector<float>& _texture_coords, const Triangles_Vec& _triangles)
{
    _texture_coords.clear();
    _texture_coords.resize(_triangles.size() * 6);

    for(unsigned int t_i = 0; t_i < _triangles.size(); ++t_i)
    {
        const Triangle& triangle = _triangles[t_i];

        const glm::vec3& point_0 = m_points_cache[ triangle.id[0] ];
        const glm::vec3& point_1 = m_points_cache[ triangle.id[1] ];
        const glm::vec3& point_2 = m_points_cache[ triangle.id[2] ];

        glm::vec3 triangle_normal = LST::Math::cross_product(point_1 - point_0, point_2 - point_0);
        LST::Math::shrink_vector_to_1(triangle_normal);

        glm::vec3 u_axis = point_1 - point_0;
        LST::Math::shrink_vector_to_1(u_axis);

        glm::vec3 v_axis = LST::Math::cross_product(triangle_normal, u_axis);
        LST::Math::shrink_vector_to_1(v_axis);

        _texture_coords.push(0.0f);
        _texture_coords.push(0.0f);

        _texture_coords.push( LST::Math::dot_product(point_1 - point_0, u_axis) / m_extraction_cell_size );
        _texture_coords.push(0.0f);

        _texture_coords.push( LST::Math::dot_product(point_2 - point_0, u_axis) / m_extraction_cell_size );
        _texture_coords.push( LST::Math::dot_product(point_2 - point_0, v_axis) / m_extraction_cell_size );
    }
}

void Mesh_Extractor_3D::M_extract_normals_data(LDS::Vector<float>& _normals, const Triangles_Vec& _triangles)
{
    _normals.clear();
    _normals.resize(_triangles.size() * 9);

    for(unsigned int t_i = 0; t_i < _triangles.size(); ++t_i)
    {
        const Triangle& triangle = _triangles[t_i];

        const glm::vec3& point_0 = m_points_cache[ triangle.id[0] ];
        const glm::vec3& point_1 = m_points_cache[ triangle.id[1] ];
        const glm::vec3& point_2 = m_points_cache[ triangle.id[2] ];

        glm::vec3 normal = LST::Math::cross_product(point_1 - point_0, point_2 - point_0);
        LST::Math::shrink_vector_to_1(normal);

        for(unsigned int r_i = 0; r_i < 3; ++r_i)
        {
            _normals.push(normal.x);
            _normals.push(normal.y);
            _normals.push(normal.z);
        }
    }
}

void Mesh_Extractor_3D::M_extract_meshes()
{
    m_voxel_meshes_map.clear();

    for(Voxel_Triangles_Map::Iterator it = m_voxel_triangles.iterator(); !it.end_reached(); ++it)
    {
        const LST::Signed_Coordinates& coords = it.key();
        const Triangles_Vec& triangles = *it;

        L_ASSERT(triangles.size() > 0);
        L_ASSERT(!m_voxel_meshes_map.contains(coords));

        Voxel_Meshes_Map::Iterator mesh_it = m_voxel_meshes_map.insert_and_get_iterator(coords, {});
        Mesh_3D& mesh = *mesh_it;

        M_extract_geometry_data(mesh.geometry, triangles);
        M_extract_texture_coords_data(mesh.texture_coordinates, triangles);
        M_extract_normals_data(mesh.normals, triangles);
    }
}



void Mesh_Extractor_3D::clear()
{
    m_ids_cache.clear();
    m_voxel_triangles.clear();
}

void Mesh_Extractor_3D::extract()
{
    L_ASSERT(m_voxel_controller);

    m_extraction_cell_size = m_voxel_controller->voxel_size() / powf(2.0f, (float)m_max_extraction_depth);

    Chunk_3D_Layer layer;
    layer.set_voxel_controller(m_voxel_controller);
    layer.set_max_depth(m_max_extraction_depth);
    layer.reload();

    bool last_layer_reached = false;

    LST::Quantized_Vector::set_factor(m_extraction_cell_size * 0.1f);

    while(!last_layer_reached)
    {
        for(Chunk_3D_Layer::Layer_Map::Const_Iterator it = layer.current_layer().iterator(); !it.end_reached(); ++it)
            M_append_mesh_data(it.key(), it->points());

        last_layer_reached = !layer.load_next_layer();
    }

    M_extract_meshes();
}



const Mesh_3D* Mesh_Extractor_3D::get_mesh(const LST::Signed_Coordinates& _coords) const
{
    Voxel_Meshes_Map::Const_Iterator it = m_voxel_meshes_map.find(_coords);
    if(!it.is_ok())
        return nullptr;
    return &(*it);
}
