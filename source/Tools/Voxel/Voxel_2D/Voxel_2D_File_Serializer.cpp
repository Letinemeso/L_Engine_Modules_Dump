#include <Tools/Voxel/Voxel_2D/Voxel_2D_File_Serializer.h>

#include <Stuff/File.h>

using namespace LMD;


Voxel_2D_File_Serializer::Voxel_2D_File_Serializer(const Voxel_Constructor_Function& _voxel_constructor_function, const std::string& _file_path_format)
    : m_voxel_constructor(_voxel_constructor_function), m_file_path_format(_file_path_format)
{

}

Voxel_2D_File_Serializer::~Voxel_2D_File_Serializer()
{

}



void Voxel_2D_File_Serializer::save_voxel(const Voxel_2D* _voxel, int _index_x, int _index_y) const
{
    std::string file_path = m_file_path_format + '_' + std::to_string(_index_x) + '_' + std::to_string(_index_y) + ".vch";
    LST::File file(file_path);

    std::string serialized_data = _voxel->serialize_data();

    file.clear();
    file.append_block(serialized_data);
}

Voxel_2D* Voxel_2D_File_Serializer::load_voxel(int _index_x, int _index_y) const
{
    std::string file_path = m_file_path_format + '_' + std::to_string(_index_x) + '_' + std::to_string(_index_y) + ".vch";
    LST::File file(file_path);

    if(!file.exists())
        return nullptr;

    std::string serialized_data = file.extract_block();

    unsigned int offset = 0;
    Voxel_2D* result = m_voxel_constructor(_index_x, _index_y);

    result->deserialize_data(serialized_data);

    return result;
}
