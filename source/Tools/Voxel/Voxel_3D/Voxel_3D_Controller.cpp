#include <Tools/Voxel/Voxel_3D/Voxel_3D_Controller.h>

#include <Stuff/Cast_Tools.h>

using namespace LMD;


Voxel_3D_Controller::Voxel_3D_Controller(float _voxel_size)
{
    m_voxel_size = _voxel_size;
    m_voxel_size_inverted = 1.0f / _voxel_size;
}

Voxel_3D_Controller::Voxel_3D_Controller(const Voxel_3D_Controller& _other)
{
    m_voxel_size = _other.m_voxel_size;
    m_voxel_size_inverted = _other.m_voxel_size_inverted;

    for(Voxel_Map::Const_Iterator it = _other.m_voxels.iterator(); !it.end_reached(); ++it)
    {
        Voxel_3D* voxel = new Voxel_3D(**it);
        m_voxels.insert( it.key(), voxel );
    }
}

Voxel_3D_Controller::Voxel_3D_Controller(Voxel_3D_Controller&& _from)
{
    m_voxel_size = _from.m_voxel_size;
    m_voxel_size_inverted = _from.m_voxel_size_inverted;

    m_voxels = LST::move(_from.m_voxels);
}

void Voxel_3D_Controller::operator=(const Voxel_3D_Controller& _other)
{
    clear();

    m_voxel_size = _other.m_voxel_size;
    m_voxel_size_inverted = _other.m_voxel_size_inverted;

    for(Voxel_Map::Const_Iterator it = _other.m_voxels.iterator(); !it.end_reached(); ++it)
    {
        Voxel_3D* voxel = new Voxel_3D(**it);
        m_voxels.insert( it.key(), voxel );
    }
}

void Voxel_3D_Controller::operator=(Voxel_3D_Controller&& _from)
{
    clear();

    m_voxel_size = _from.m_voxel_size;
    m_voxel_size_inverted = _from.m_voxel_size_inverted;

    m_voxels = LST::move(_from.m_voxels);
}

Voxel_3D_Controller::~Voxel_3D_Controller()
{
    clear();
}



void Voxel_3D_Controller::clear()
{
    for(Voxel_Map::Iterator it = m_voxels.iterator(); !it.end_reached(); ++it)
        delete *it;

    m_voxels.clear();
}



LST::Signed_Coordinates Voxel_3D_Controller::calculate_coordinates(const glm::vec3& _at) const
{
    LST::Signed_Coordinates result;

    for(unsigned int i = 0; i < 3; ++i)
    {
        int component = _at[i] * m_voxel_size_inverted;
        if(_at[i] < 0.0f)
            --component;

        result[i] = component;
    }

    return result;
}


Voxel_3D* Voxel_3D_Controller::get_voxel(const LST::Signed_Coordinates& _coordinates) const
{
    Voxel_Map::Const_Iterator maybe_voxel = m_voxels.find(_coordinates);
    if(!maybe_voxel.is_ok())
        return nullptr;
    return *maybe_voxel;
}

Voxel_3D* Voxel_3D_Controller::get_voxel(const glm::vec3& _at) const
{
    LST::Signed_Coordinates coordinates = calculate_coordinates(_at);
    return get_voxel(coordinates);
}

Voxel_3D* Voxel_3D_Controller::get_or_insert_voxel(const LST::Signed_Coordinates& _coordinates)
{
    Voxel_Map::Iterator voxel_it = m_voxels.find_or_insert(_coordinates);
    *voxel_it = new Voxel_3D();
    return *voxel_it;
}

Voxel_3D* Voxel_3D_Controller::get_or_insert_voxel(const glm::vec3& _at)
{
    LST::Signed_Coordinates coordinates = calculate_coordinates(_at);
    return get_or_insert_voxel(coordinates);
}
