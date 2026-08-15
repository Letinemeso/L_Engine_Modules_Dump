#pragma once

#include <vec3.hpp>

#include <L_Debug/L_Debug.h>


namespace LMD
{

    class Voxel_3D final
    {
    public:
        constexpr static unsigned int Subvoxels_Amount = 8;
        enum Subvoxel_Position : unsigned int
        {
            Left_Bottom_Close = 0,
            Right_Bottom_Close = 1,
            Left_Top_Close = 2,
            Right_Top_Close = 3,
            Left_Bottom_Far = 4,
            Right_Bottom_Far = 5,
            Left_Top_Far = 6,
            Right_Top_Far = 7,
        };

    private:
        Voxel_3D* m_subvoxels[Subvoxels_Amount] = { nullptr };
        unsigned int m_depth = 0;

        unsigned int m_value = 0;   // material or something like this
        glm::vec3 m_offset = {0.0f, 0.0f, 0.0f};
        float m_size = 0.0f;

    public:
        Voxel_3D();
        Voxel_3D(unsigned int _depth);
        Voxel_3D(const Voxel_3D& _other);
        Voxel_3D(Voxel_3D&& _from);
        void operator=(const Voxel_3D& _other);
        void operator=(Voxel_3D&& _from);

        ~Voxel_3D();

    public:
        inline void set_value(unsigned int _value) { m_value = _value; }
        inline void set_size(float _value) { m_size = _value; }
        inline void set_offset(const glm::vec3& _value) { m_offset = _value; }

        inline Voxel_3D* child(unsigned int _index) { L_ASSERT(_index < Subvoxels_Amount); return m_subvoxels[_index]; }
        inline const Voxel_3D* child(unsigned int _index) const { L_ASSERT(_index < Subvoxels_Amount); return m_subvoxels[_index]; }
        inline unsigned int depth() const { return m_depth; }
        inline bool is_split() const { return m_subvoxels[0] != nullptr; }

        inline unsigned int value() const { return m_value; }
        inline float size() const { return m_size; }
        inline const glm::vec3& offset() const { return m_offset; }
        inline glm::vec3 calculate_center() const { return m_offset + m_size; }

    public:
        void split();
        void merge();

    };

}
