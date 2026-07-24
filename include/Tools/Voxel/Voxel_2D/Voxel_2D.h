#pragma once

#include <L_Debug/L_Debug.h>


namespace LGL
{

    class Voxel_2D
    {
    public:
        enum Subvoxel_Position : unsigned int
        {
            Bottom_Left = 0,
            Bottom_Right = 1,
            Top_Left = 2,
            Top_Right = 3
        };

    protected:
        unsigned int m_max_depth = 1;
        unsigned int m_depth = 0;

        unsigned int m_id = 0;

        float m_position_x = 0.0f;
        float m_position_y = 0.0f;

        float m_size_x = 0.0f;
        float m_size_y = 0.0f;

    protected:
        Voxel_2D* m_parent = nullptr;
        Voxel_2D* m_childs[4] { nullptr };

    private:
        Voxel_2D() { }

    public:
        Voxel_2D(Voxel_2D* _parent, unsigned int _max_depth, unsigned int _depth, unsigned int _id, float _position_x, float _position_y, float _size_x, float _size_y);
        ~Voxel_2D();

    public:
        virtual Voxel_2D* construct_copy() const;

    public:
        inline void set_id(unsigned int _value) { m_id = _value; }

        inline float position_x() const { return m_position_x; }
        inline float position_y() const { return m_position_y; }

        inline float size_x() const { return m_size_x; }
        inline float size_y() const { return m_size_y; }

        inline unsigned int max_depth() const { return m_max_depth; }
        inline unsigned int depth() const { return m_depth; }
        inline unsigned int id() const { return m_id; }

        inline Voxel_2D* parent() { return m_parent; }
        inline const Voxel_2D* parent() const { return m_parent; }
        inline Voxel_2D* child(unsigned int _index) { L_ASSERT(_index < 4); return m_childs[_index]; }
        inline const Voxel_2D* child(unsigned int _index) const { L_ASSERT(_index < 4); return m_childs[_index]; }

        inline bool reached_max_depth() const { return m_depth == m_max_depth; }
        inline bool is_split() const { return m_childs[0] != nullptr; }

    public:
        virtual bool should_be_merged() const;
        virtual bool can_be_split() const;

    protected:
        virtual void M_init_childs();
        virtual void M_before_merge();

    public:
        void split();
        void merge();
        void merge_subtrees_if_needed();

    protected:
        std::string M_parse_word(const std::string& _str, unsigned int _offset) const;

    protected:
        virtual std::string M_serialize_recursive() const;
        virtual void M_deserialize_recursive(const std::string& _str, unsigned int& _offset);

    public:
        std::string serialize_data() const;
        void deserialize_data(const std::string& _str);

    };

}
