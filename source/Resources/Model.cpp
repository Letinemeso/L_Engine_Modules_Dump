#include <Resources/Model.h>

#include <sstream>
#include <regex>

#include <vec2.hpp>
#include <vec3.hpp>

using namespace LMD;


void Model::set_geometry_data(const LDS::Vector<float>& _data)
{
    m_geometry_data = _data;
}

void Model::set_geometry_data(LDS::Vector<float>&& _data)
{
    m_geometry_data = (LDS::Vector<float>&&)_data;
}

void Model::set_texture_data(const LDS::Vector<float>& _data)
{
    m_texture_data = _data;
}

void Model::set_texture_data(LDS::Vector<float>&& _data)
{
    m_texture_data = (LDS::Vector<float>&&)_data;
}

void Model::set_normals_data(const LDS::Vector<float>& _data)
{
    m_normals_data = _data;
}

void Model::set_normals_data(LDS::Vector<float>&& _data)
{
    m_normals_data = (LDS::Vector<float>&&)_data;
}





namespace LMD
{
    struct Polygon_Data
    {
        constexpr static unsigned int Points = 0;
        constexpr static unsigned int Texture_Coords = 1;
        constexpr static unsigned int Normals = 2;

        unsigned int indices[3][3];

        unsigned int* point_indices = indices[Points];
        unsigned int* texture_coord_indices = indices[Texture_Coords];
        unsigned int* normal_indices = indices[Normals];
    };

    struct Model_Data
    {
        LDS::Vector<glm::vec3> points;
        LDS::Vector<glm::vec2> texture_coords;
        LDS::Vector<glm::vec3> normals;
        LDS::Vector<Polygon_Data> polygons;

        Model_Data() { }

        Model_Data(Model_Data&& _from)
        {
            points = (LDS::Vector<glm::vec3>&&)_from.points;
            texture_coords = (LDS::Vector<glm::vec2>&&)_from.texture_coords;
            normals = (LDS::Vector<glm::vec3>&&)_from.normals;
            polygons = (LDS::Vector<Polygon_Data>&&)_from.polygons;
        }

        void operator=(Model_Data&& _from)
        {
            points = (LDS::Vector<glm::vec3>&&)_from.points;
            texture_coords = (LDS::Vector<glm::vec2>&&)_from.texture_coords;
            normals = (LDS::Vector<glm::vec3>&&)_from.normals;
            polygons = (LDS::Vector<Polygon_Data>&&)_from.polygons;
        }
    };


    glm::vec2 parse_obj_vec2(std::istringstream& _line_as_stream)
    {
        glm::vec2 result;
        _line_as_stream >> result.x >> result.y;
        return result;
    }

    glm::vec3 parse_obj_vec3(std::istringstream& _line_as_stream)
    {
        glm::vec3 result;
        _line_as_stream >> result.x >> result.y >> result.z;
        return result;
    }

    Polygon_Data parse_obj_polygon(std::istringstream& _line_as_stream)
    {
        Polygon_Data result;

        for(unsigned int i = 0; i < 3; ++i)
        {
            unsigned int point_index = 0;
            unsigned int texture_coord_index = 0;
            unsigned int normal_index = 0;

            _line_as_stream >> point_index;
            _line_as_stream.get();
            _line_as_stream >> texture_coord_index;
            _line_as_stream.get();
            _line_as_stream >> normal_index;
            _line_as_stream.get();

            result.point_indices[i] = point_index - 1;
            result.texture_coord_indices[i] = texture_coord_index - 1;
            result.normal_indices[i] = normal_index - 1;
        }

        return result;
    }

    Model_Data parse_obj(const std::string& _file_name)
    {
        std::ifstream file(_file_name, std::ios::binary);
        L_ASSERT(file.is_open());

        Model_Data result;

        for (std::string line; std::getline(file, line); )
        {
            std::istringstream line_as_stream(line);

            std::string command;
            line_as_stream >> command;

            if(command[0] == '#')
                continue;

            if(command == "v")
                result.points.push(parse_obj_vec3(line_as_stream));
            else if(command == "vn")
                result.normals.push(parse_obj_vec3(line_as_stream));
            else if(command == "vt")
                result.texture_coords.push(parse_obj_vec2(line_as_stream));
            else if(command == "f")
                result.polygons.push(parse_obj_polygon(line_as_stream));
        }

        return result;
    }


    template<typename _Vector_Type>
    void extract_raw_data(LDS::Vector<float>& _result, unsigned int _floats_per_vertex, const LDS::Vector<_Vector_Type>& _data_vec, const LDS::Vector<Polygon_Data>& _polygons_data, unsigned int _data_type_index)
    {
        constexpr unsigned int Vertices_Per_Polygon = 3;
        const unsigned int Floats_Per_Polygon = Vertices_Per_Polygon * _floats_per_vertex;

        unsigned int geometry_data_size = _polygons_data.size() * Floats_Per_Polygon;
        _result.mark_empty();
        _result.resize_and_fill(geometry_data_size, 0.0f);

        for(unsigned int polygon_i = 0; polygon_i < _polygons_data.size(); ++polygon_i)
        {
            const Polygon_Data& polygon_data = _polygons_data[polygon_i];
            const unsigned int* indices = polygon_data.indices[_data_type_index];

            unsigned int gd_offset = Floats_Per_Polygon * polygon_i;
            float* gd_w_offset = _result.raw_data() + gd_offset;

            for(unsigned int vertex_i = 0; vertex_i < Vertices_Per_Polygon; ++vertex_i)
            {
                const auto& vertex = _data_vec[indices[vertex_i]];

                unsigned int vertex_offset = vertex_i * _floats_per_vertex;

                for(unsigned int i = 0; i < _floats_per_vertex; ++i)
                    gd_w_offset[vertex_offset + i] = vertex[i];
            }
        }
    }
}



ON_VALUES_ASSIGNED_IMPLEMENTATION(Model_Stub)
{
    L_ASSERT(file_path.size() > 0);

    constexpr const char* Obj_Format = ".obj";

    auto check_format = [this](const char* _format)->bool
    {
        if(file_path.size() < strlen(_format))
            return false;

        return file_path.substr(file_path.size() - strlen(_format)) == _format;
    };

    Model_Data model_data;

    if(check_format(Obj_Format))
        model_data = parse_obj(file_path);
    else
    {
        L_ASSERT(false && "unknown model file format");
    }

    L_ASSERT(model_data.points.size() > 0);

    extract_raw_data(geometry_data, 3, model_data.points, model_data.polygons, Polygon_Data::Points);
    extract_raw_data(normals_data, 3, model_data.normals, model_data.polygons, Polygon_Data::Normals);

    if(model_data.texture_coords.size() > 1)
        extract_raw_data(texture_data, 2, model_data.texture_coords, model_data.polygons, Polygon_Data::Texture_Coords);
}



BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Model_Stub)

BUILDER_STUB_INITIALIZATION_FUNC(Model_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    L_ASSERT(geometry_data.size() > 0);

    product->set_geometry_data(geometry_data);
    product->set_normals_data(geometry_data);

    if(texture_data.size() == 0)
        return;

    product->set_texture_data(texture_data);
}
