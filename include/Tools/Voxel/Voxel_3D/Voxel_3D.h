#pragma once


namespace LMD
{

    class Voxel_3D final
    {
    private:
        constexpr static unsigned int Subvoxels_Amount = 8;

    private:
        Voxel_3D* m_child_voxels[Subvoxels_Amount] = { nullptr };
        unsigned int m_depth = 0;

    public:
        Voxel_3D();
        Voxel_3D(const Voxel_3D& _other);
        Voxel_3D(Voxel_3D&& _from);
        void operator=(const Voxel_3D& _other);
        void operator=(Voxel_3D&& _from);

        ~Voxel_3D();

    public:
        inline Voxel_3D
        inline unsigned int depth() const { return m_depth; }

    public:
        void split();
        void merge();

    };

}
