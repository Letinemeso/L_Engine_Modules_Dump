#pragma once

#include <Data_Structures/List.h>
#include <Stuff/Function_Wrapper.h>

#include <Tools/Voxel/Voxel_2D/Voxel_2D.h>
#include <Tools/Voxel/Voxel_2D/Voxel_2D_Serializer.h>
#include <Tools/Voxel/Voxel_2D/Voxel_2D_Generator.h>
#include <Tools/Voxel/Voxel_2D/Voxel_2D_Modification_Tools.h>


namespace LGL
{

    class Voxel_2D_Controller
    {
    public:
        struct Voxel_Data
        {
            int index_x = 0;
            int index_y = 0;
            Voxel_2D* voxel = nullptr;
            bool changes_were_made = false;
        };

    public:
        using Voxel_List = LDS::List<Voxel_Data>;
        using Voxel_Action_Callback_Func = LST::Function<void(const Voxel_Data&)>;
        using World_Center_Changed_Callback_Func = LST::Function<void()>;

    private:
        Voxel_List m_voxels;
        int m_current_world_center_x = 0;
        int m_current_world_center_y = 0;

        Voxel_Action_Callback_Func m_on_voxel_added;
        Voxel_Action_Callback_Func m_on_voxel_removed;
        World_Center_Changed_Callback_Func m_on_world_center_changed;

        float m_expected_voxel_size_x = 0.0f;
        float m_expected_voxel_size_y = 0.0f;
        unsigned int m_expected_max_depth = 0;

    private:
        Voxel_2D_Serializer* m_serializer = nullptr;
        Voxel_2D_Generator* m_generator = nullptr;

        unsigned int m_loaded_voxels_amount_from_center = 2;

    public:
        Voxel_2D_Controller();
        ~Voxel_2D_Controller();

    public:
        inline const Voxel_List& voxel_list() const { return m_voxels; }

        inline void set_on_voxel_added_callback(const Voxel_Action_Callback_Func& _value) { m_on_voxel_added = _value; }
        inline void set_on_voxel_removed_callback(const Voxel_Action_Callback_Func& _value) { m_on_voxel_removed = _value; }
        inline void set_on_world_center_changed_callback(const World_Center_Changed_Callback_Func& _value) { m_on_world_center_changed = _value; }
        inline void set_expected_voxel_size(float _x, float _y) { m_expected_voxel_size_x = _x; m_expected_voxel_size_y = _y; }
        inline void set_expected_max_depth(unsigned int _value) { m_expected_max_depth = _value; }
        inline void set_generator(Voxel_2D_Generator* _ptr) { delete m_generator; m_generator = _ptr; }
        inline void set_serializer(Voxel_2D_Serializer* _ptr) { delete m_serializer; m_serializer = _ptr; }
        inline void set_loaded_voxels_amount_from_center(unsigned int _value) { m_loaded_voxels_amount_from_center = _value; }

        inline float expected_voxel_size_x() const { return m_expected_voxel_size_x; }
        inline float expected_voxel_size_y() const { return m_expected_voxel_size_y; }
        inline int world_center_x() const { return m_current_world_center_x; }
        inline int world_center_y() const { return m_current_world_center_y; }
        inline int world_offset_x() const { return m_current_world_center_x - (int)m_loaded_voxels_amount_from_center; }
        inline int world_offset_y() const { return m_current_world_center_y - (int)m_loaded_voxels_amount_from_center; }
        inline unsigned int expected_max_depth() const { return m_expected_max_depth; }
        inline unsigned int voxels_from_center() const { return m_loaded_voxels_amount_from_center; }
        inline unsigned int voxels_in_row() const { return m_loaded_voxels_amount_from_center + m_loaded_voxels_amount_from_center; }
        inline Voxel_2D_Generator* generator() { return m_generator; }
        inline Voxel_2D_Serializer* serializer() { return m_serializer; }

    private:
        Voxel_List::Iterator M_find_voxel(const Voxel_2D* _voxel);
        Voxel_List::Iterator M_find_voxel(int _index_x, int _index_y);
        std::string M_construct_save_file_name_format() const;
        int M_calculate_world_center_index(float _world_center, float _voxel_size) const;

    public:
        void remove_all_voxels();
        void reload_voxels(float _world_center_x, float _world_center_y);
        void reload_voxels(bool _should_save);
        void save_voxels();
        void update_world_center(float _world_center_x, float _world_center_y);

    public:
        void insert_voxel(Voxel_2D* _voxel);
        void remove_voxel(Voxel_2D* _voxel);
        void remove_voxel(int _index_x, int _index_y);
        void reload_voxel_if_registered(int _index_x, int _index_y);

    public:
        void process_logic_for_voxels(const Voxel_Modification_Func& _voxel_modification_func);

    public:
        void mark_all_unchanged();

    };

}
