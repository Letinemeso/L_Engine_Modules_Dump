#include <Tools/Mesh_Generation/3D/Utility/Chunk_3D_Generation_Data.h>

#include <Stuff/Cast_Tools.h>
#include <Stuff/Math_Stuff.h>

using namespace LMD;


Chunk_3D_Generation_Data::Chunk_3D_Generation_Data(const Voxel_3D* _voxel, unsigned int _max_depth, unsigned int _default_value)
    : m_voxel(_voxel)
{
    L_ASSERT(_voxel);

    m_grid.construct_grid(*_voxel, _max_depth, _default_value);

    m_cell_size = m_voxel->size() / (float)m_grid.grid_size();
    m_cell_size_halved = m_cell_size * 0.5f;
}

Chunk_3D_Generation_Data::Chunk_3D_Generation_Data(const Chunk_3D_Generation_Data& _other)
{
    m_voxel = _other.m_voxel;
    m_grid = _other.m_grid;
    m_points = _other.m_points;

    m_cell_size = _other.m_cell_size;
    m_cell_size_halved = _other.m_cell_size_halved;
}

Chunk_3D_Generation_Data::Chunk_3D_Generation_Data(Chunk_3D_Generation_Data&& _from)
    : m_voxel(_from.m_voxel)
{
    m_grid = LST::move(_from.m_grid);
    m_points = LST::move(_from.m_points);

    m_cell_size = _from.m_cell_size;
    m_cell_size_halved = _from.m_cell_size_halved;
}

void Chunk_3D_Generation_Data::operator=(const Chunk_3D_Generation_Data& _other)
{
    m_voxel = _other.m_voxel;
    m_grid = _other.m_grid;
    m_points = _other.m_points;

    m_cell_size = _other.m_cell_size;
    m_cell_size_halved = _other.m_cell_size_halved;
}

void Chunk_3D_Generation_Data::operator=(Chunk_3D_Generation_Data&& _from)
{
    m_voxel = _from.m_voxel;
    m_grid = LST::move(_from.m_grid);
    m_points = LST::move(_from.m_points);

    m_cell_size = _from.m_cell_size;
    m_cell_size_halved = _from.m_cell_size_halved;
}



void Chunk_3D_Generation_Data::M_append_quad(const glm::vec3& _point_0, const glm::vec3& _point_1, const glm::vec3& _point_2, const glm::vec3& _point_3)
{
    m_points.push(_point_3);
    m_points.push(_point_2);
    m_points.push(_point_0);

    m_points.push(_point_2);
    m_points.push(_point_1);
    m_points.push(_point_0);
}

void Chunk_3D_Generation_Data::M_append_cell_data(const Neighbors_Data& _neighbors, const LST::Coordinates& _coords)
{
    int x = _coords.x();
    int y = _coords.y();
    int z = _coords.z();

    if(m_grid.value_at(x, y, z) != 0)
        return;

    glm::vec3 center = m_voxel->offset() + glm::vec3(
                                                     (float)_coords.x() * m_cell_size,
                                                     (float)_coords.y() * m_cell_size,
                                                     (float)_coords.z() * m_cell_size
                                                    );

    /*

       6___________7
      /|          /|
    4/__________5/ |
    |  |        |  |
    |  |        |  |
    |  2________| _3
    | /         | /
    |/__________|/
    0           1

    */

    /* 0 */ glm::vec3 left_down_near = center +  glm::vec3(-m_cell_size_halved, -m_cell_size_halved, -m_cell_size_halved);
    /* 1 */ glm::vec3 right_down_near = center + glm::vec3( m_cell_size_halved, -m_cell_size_halved, -m_cell_size_halved);
    /* 2 */ glm::vec3 left_down_far = center +   glm::vec3(-m_cell_size_halved, -m_cell_size_halved,  m_cell_size_halved);
    /* 3 */ glm::vec3 right_down_far = center +  glm::vec3( m_cell_size_halved, -m_cell_size_halved,  m_cell_size_halved);
    /* 4 */ glm::vec3 left_up_near = center +    glm::vec3(-m_cell_size_halved,  m_cell_size_halved, -m_cell_size_halved);
    /* 5 */ glm::vec3 right_up_near = center +   glm::vec3( m_cell_size_halved,  m_cell_size_halved, -m_cell_size_halved);
    /* 6 */ glm::vec3 left_up_far = center +     glm::vec3(-m_cell_size_halved,  m_cell_size_halved,  m_cell_size_halved);
    /* 7 */ glm::vec3 right_up_far = center +    glm::vec3( m_cell_size_halved,  m_cell_size_halved,  m_cell_size_halved);

    if(m_grid.value_at(x - 1, y, z) != 0)
        M_append_quad(left_down_far, left_up_far, left_up_near, left_down_near);
    if(m_grid.value_at(x + 1, y, z) != 0)
        M_append_quad(right_down_near, right_up_near, right_up_far, right_down_far);
    if(m_grid.value_at(x, y - 1, z) != 0)
        M_append_quad(right_down_near, right_down_far, left_down_far, left_down_near);
    if(m_grid.value_at(x, y + 1, z) != 0)
        M_append_quad(right_up_far, right_up_near, left_up_near, left_up_far);
    if(m_grid.value_at(x, y, z - 1) != 0)
        M_append_quad(left_down_near, left_up_near, right_up_near, right_down_near);
    if(m_grid.value_at(x, y, z + 1) != 0)
        M_append_quad(right_up_far, left_up_far, left_down_far, right_down_far);
}


void Chunk_3D_Generation_Data::M_fill_grid_halo(const Neighbors_Data& _neighbors)
{
    if(_neighbors.data[0][1][1])
        m_grid.fill_halo_left(_neighbors.data[0][1][1]->grid());

    if(_neighbors.data[2][1][1])
        m_grid.fill_halo_right(_neighbors.data[2][1][1]->grid());


    if(_neighbors.data[1][0][1])
        m_grid.fill_halo_bottom(_neighbors.data[1][0][1]->grid());

    if(_neighbors.data[1][2][1])
        m_grid.fill_halo_top(_neighbors.data[1][2][1]->grid());


    if(_neighbors.data[1][1][0])
        m_grid.fill_halo_back(_neighbors.data[1][1][0]->grid());

    if(_neighbors.data[1][1][2])
        m_grid.fill_halo_front(_neighbors.data[1][1][2]->grid());
}



void Chunk_3D_Generation_Data::extract_points(const Neighbors_Data& _neighbors)
{
    L_ASSERT(m_voxel);

    m_points.clear();

    M_fill_grid_halo(_neighbors);

    LST::Coordinates coords;
    for(coords.z() = 0; coords.z() < m_grid.grid_size(); ++coords.z())
    {
        for(coords.y() = 0; coords.y() < m_grid.grid_size(); ++coords.y())
        {
            for(coords.x() = 0; coords.x() < m_grid.grid_size(); ++coords.x())
            {
                M_append_cell_data(_neighbors, coords);
            }
        }
    }
}
