#pragma once

#include <Stuff/Function_Wrapper.h>

#include <Tools/Voxel/Voxel_2D/Voxel_2D.h>
#include <Tools/Voxel/Voxel_2D/Voxel_2D_Serializer.h>


namespace LMD
{

    class Voxel_2D_File_Serializer : public Voxel_2D_Serializer
    {
    public:
        using Voxel_Constructor_Function = LST::Function<Voxel_2D*(int, int)>;

    private:
        Voxel_Constructor_Function m_voxel_constructor;
        std::string m_file_path_format;

    public:
        Voxel_2D_File_Serializer(const Voxel_Constructor_Function& _voxel_constructor_function, const std::string& _file_path_format);
        ~Voxel_2D_File_Serializer();

    public:
        inline void set_file_path_format(const std::string& _value) { m_file_path_format = _value; }
        inline const std::string& file_path_format() const { return m_file_path_format; }

    public:
        void save_voxel(const Voxel_2D* _voxel, int _index_x, int _index_y) const override;
        Voxel_2D* load_voxel(int _index_x, int _index_y) const override;

    };

}
