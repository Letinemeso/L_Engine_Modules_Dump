#include <Resources/Model.h>

#include <sstream>
#include <regex>

#include <vec2.hpp>
#include <vec3.hpp>

using namespace LMD;


Model::Model()
{

}

Model::~Model()
{
    delete[] m_geometry_data;
    delete[] m_texture_data;
}



void Model::copy_geometry_data(const float* _raw_data, unsigned int _raw_data_size)
{
    delete[] m_geometry_data;

    m_geometry_data_size = _raw_data_size;
    m_geometry_data = new float[m_geometry_data_size];
    for(unsigned int i = 0; i < m_geometry_data_size; ++i)
        m_geometry_data[i] = _raw_data[i];
}

void Model::use_geometry_data(float* _raw_data, unsigned int _raw_data_size)
{
    delete[] m_geometry_data;

    m_geometry_data_size = _raw_data_size;
    m_geometry_data = _raw_data;
}

void Model::copy_texture_data(const float* _raw_data, unsigned int _raw_data_size)
{
    delete[] m_texture_data;

    m_texture_data_size = _raw_data_size;
    m_texture_data = new float[m_texture_data_size];
    for(unsigned int i = 0; i < m_texture_data_size; ++i)
        m_texture_data[i] = _raw_data[i];
}

void Model::use_texture_data(float* _raw_data, unsigned int _raw_data_size)
{
    delete[] m_texture_data;

    m_texture_data_size = _raw_data_size;
    m_texture_data = _raw_data;
}





namespace LMD
{
    struct Polygon_Data
    {
        unsigned int point_indices[3];
        unsigned int texture_coord_indices[3];
        unsigned int normal_indices[3];
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
        static const std::regex pattern(R"((\d+)/(\d+)/(\d+))");

        Polygon_Data result;

        for(unsigned int i = 0; i < 3; ++i)
        {
            std::string polygon_as_str;
            _line_as_stream >> polygon_as_str;

            std::smatch matches;
            bool match = std::regex_match(polygon_as_str, matches, pattern);
            L_ASSERT(match);

            result.point_indices[i] = std::stoi(matches[0]) - 1;
            result.texture_coord_indices[i] = std::stoi(matches[1]) - 1;
            result.normal_indices[i] = std::stoi(matches[2]) - 1;
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

    constexpr unsigned int Coords_Per_Polygon = 3 * 3;
    constexpr unsigned int Texture_Coords_Per_Polygon = 3 * 2;

    delete[] geometry_data;
    geometry_data_size = model_data.polygons.size() * Coords_Per_Polygon;
    geometry_data = new float[geometry_data_size];

    for(unsigned int polygon_i = 0; polygon_i < model_data.polygons.size(); ++polygon_i)
    {
        const Polygon_Data& polygon_data = model_data.polygons[polygon_i];

        unsigned int gd_offset = Coords_Per_Polygon * polygon_i;
        float* gd_w_offset = geometry_data + gd_offset;

        for(unsigned int vertex_i = 0; vertex_i < 3; ++vertex_i)
        {
            const glm::vec3& vertex = model_data.points[polygon_data.point_indices[vertex_i]];

            unsigned int vertex_offset = vertex_i * 3;

            for(unsigned int i = 0; i < 3; ++i)
                gd_w_offset[vertex_offset + i] = vertex[i];
        }
    }

    delete[] texture_data;

    if(model_data.texture_coords.size() == 0)
        return;

    texture_data_size = model_data.polygons.size() * Texture_Coords_Per_Polygon;
    texture_data = new float[texture_data_size];

    for(unsigned int polygon_i = 0; polygon_i < model_data.polygons.size(); ++polygon_i)
    {
        const Polygon_Data& polygon_data = model_data.polygons[polygon_i];

        unsigned int tcd_offset = Texture_Coords_Per_Polygon * polygon_i;
        float* tcd_w_offset = texture_data + tcd_offset;

        for(unsigned int vertex_i = 0; vertex_i < 3; ++vertex_i)
        {
            const glm::vec2& vertex = model_data.texture_coords[polygon_data.texture_coord_indices[vertex_i]];

            unsigned int vertex_offset = vertex_i * 2;

            for(unsigned int i = 0; i < 2; ++i)
                tcd_w_offset[vertex_offset + i] = vertex[i];
        }
    }
}



Model_Stub::~Model_Stub()
{
    delete[] geometry_data;
    delete[] texture_data;
}



BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Model_Stub)

BUILDER_STUB_INITIALIZATION_FUNC(Model_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    L_ASSERT(geometry_data);
    L_ASSERT(geometry_data_size > 0);

    product->copy_geometry_data(geometry_data, geometry_data_size);

    if(texture_data_size == 0 && !texture_data)
        return;

    L_ASSERT(texture_data);
    L_ASSERT(texture_data_size > 0);

    product->copy_texture_data(texture_data, texture_data_size);
}
