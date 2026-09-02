#include <Tools/Voxel/Voxel_3D/Voxel_3D.h>

using namespace LMD;


Voxel_3D::Voxel_3D()
{

}

Voxel_3D::Voxel_3D(unsigned int _depth)
{
    m_depth = _depth;
}

Voxel_3D::Voxel_3D(const Voxel_3D& _other)
{
    m_depth = _other.m_depth;
    m_value = _other.m_value;
    m_size = _other.m_size;
    m_offset = _other.m_offset;

    if(!_other.is_split())
        return;

    for(unsigned int i = 0; i < Subvoxels_Amount; ++i)
        m_subvoxels[i] = new Voxel_3D( *_other.m_subvoxels[i] );
}

Voxel_3D::Voxel_3D(Voxel_3D&& _from)
{
    m_depth = _from.m_depth;
    m_value = _from.m_value;
    m_size = _from.m_size;
    m_offset = _from.m_offset;

    for(unsigned int i = 0; i < Subvoxels_Amount; ++i)
    {
        m_subvoxels[i] = _from.m_subvoxels[i];
        _from.m_subvoxels[i] = nullptr;
    }
}

void Voxel_3D::operator=(const Voxel_3D& _other)
{
    merge();

    m_depth = _other.m_depth;
    m_value = _other.m_value;
    m_size = _other.m_size;
    m_offset = _other.m_offset;

    if(!_other.is_split())
        return;

    for(unsigned int i = 0; i < Subvoxels_Amount; ++i)
        m_subvoxels[i] = new Voxel_3D( *_other.m_subvoxels[i] );
}

void Voxel_3D::operator=(Voxel_3D&& _from)
{
    merge();

    m_depth = _from.m_depth;
    m_value = _from.m_value;
    m_size = _from.m_size;
    m_offset = _from.m_offset;

    for(unsigned int i = 0; i < Subvoxels_Amount; ++i)
    {
        m_subvoxels[i] = _from.m_subvoxels[i];
        _from.m_subvoxels[i] = nullptr;
    }
}


Voxel_3D::~Voxel_3D()
{
    for(unsigned int i = 0; i < Subvoxels_Amount; ++i)
        delete m_subvoxels[i];
}



void Voxel_3D::split()
{
    L_ASSERT(!is_split());

    unsigned int next_depth = m_depth + 1;
    float next_size = m_size * 0.5f;

    for(unsigned int i = 0; i < Subvoxels_Amount; ++i)
    {
        Voxel_3D* new_voxel = new Voxel_3D(next_depth);
        m_subvoxels[i] = new_voxel;

        new_voxel->set_size(next_size);
        new_voxel->set_value(m_value);
    }

    m_subvoxels[Left_Bottom_Close]->set_offset(m_offset);
    m_subvoxels[Right_Bottom_Close]->set_offset(m_offset + glm::vec3(next_size, 0.0f, 0.0f));
    m_subvoxels[Left_Top_Close]->set_offset(m_offset + glm::vec3(0.0f, next_size, 0.0f));
    m_subvoxels[Right_Top_Close]->set_offset(m_offset + glm::vec3(next_size, next_size, 0.0f));
    m_subvoxels[Left_Bottom_Far]->set_offset(m_offset + glm::vec3(0.0f, 0.0f, next_size));
    m_subvoxels[Right_Bottom_Far]->set_offset(m_offset + glm::vec3(next_size, 0.0f, next_size));
    m_subvoxels[Left_Top_Far]->set_offset(m_offset + glm::vec3(0.0f, next_size, next_size));
    m_subvoxels[Right_Top_Far]->set_offset(m_offset + glm::vec3(next_size, next_size, next_size));
}

void Voxel_3D::merge()
{
    L_ASSERT(is_split());

    for(unsigned int i = 0; i < Subvoxels_Amount; ++i)
    {
        delete m_subvoxels[i];
        m_subvoxels[i] = nullptr;
    }
}
