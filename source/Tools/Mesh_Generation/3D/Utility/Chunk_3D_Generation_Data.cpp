#include <Tools/Mesh_Generation/3D/Utility/Chunk_3D_Generation_Data.h>

#include <Stuff/Cast_Tools.h>
#include <Stuff/Math_Stuff.h>

using namespace LMD;


Chunk_3D_Generation_Data::Chunk_3D_Generation_Data(const Voxel_3D* _voxel, unsigned int _max_depth)
    : m_voxel(_voxel)
{
    L_ASSERT(_voxel);

    m_grid.construct_grid(*_voxel, _max_depth);

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



Chunk_3D_Generation_Data::Neighbor_Cell_Location Chunk_3D_Generation_Data::M_calculate_neighbor_cell_location(const Neighbors_Data& _neighbors, unsigned int _x, unsigned int _y, unsigned int _z) const
{
    unsigned int coords[3] = { _x, _y, _z };
    unsigned int neighbor_coords[3];

    Neighbor_Cell_Location result;

    for(unsigned int i = 0; i < 3; ++i)
    {
        if(coords[i] < m_grid.grid_size())
        {
            neighbor_coords[i] = 1;
            result.coordinates[i] = coords[i];
        }
        else if(coords[i] == LST::Math::Max_Unsigned_Int)
        {
            neighbor_coords[i] = 0;
            result.coordinates[i] = m_grid.grid_size() - 1;
        }
        else
        {
            neighbor_coords[i] = 2;
            result.coordinates[i] = 0;
        }
    }

    if(result.coordinates[0] == 1 && result.coordinates[1] == 1 && result.coordinates[2] == 1)
        result.chunk = this;
    else
        result.chunk = _neighbors.data[neighbor_coords[0]][neighbor_coords[1]][neighbor_coords[2]];

    return result;
}

Chunk_3D_Generation_Data::Neighboring_Cells_Data Chunk_3D_Generation_Data::M_calculate_neighboring_cells_data(const Neighbors_Data& _neighbors, unsigned int _x, unsigned int _y, unsigned int _z) const
{
    unsigned int coords[3] = { _x, _y, _z };

    unsigned int offsets[3];

    for(unsigned int i = 0; i < 3; ++i)
        offsets[i] = coords[i] - 1;

    Neighboring_Cells_Data result;

    for(unsigned int x = 0; x < 3; ++x)
    {
        for(unsigned int y = 0; y < 3; ++y)
        {
            for(unsigned int z = 0; z < 3; ++z)
            {
                Neighbor_Cell_Location cell_location = M_calculate_neighbor_cell_location(_neighbors, offsets[0] + x, offsets[1] + y, offsets[2] + z);

                if(!cell_location.chunk)
                    continue;

                const Chunk_3D_Grid& expected_grid = cell_location.chunk->grid();
                result.values[x][y][z] = expected_grid.value_at(cell_location.coordinates.x(), cell_location.coordinates.y(), cell_location.coordinates.z());
            }
        }
    }

    return result;
}

void Chunk_3D_Generation_Data::M_balance_point(const Neighboring_Cells_Data& _neighbors_data, const glm::vec3& _voxel_center, glm::vec3& _point,
                     bool _positive_offset_x, bool _positive_offset_y, bool _positive_offset_z) const
{
    glm::vec3 offset = {0.0f, 0.0f, 0.0f};
    unsigned int voxels_appended = 0;

    unsigned int offset_x;
    unsigned int offset_y;
    unsigned int offset_z;
    unsigned int target_x;
    unsigned int target_y;
    unsigned int target_z;

    if(_positive_offset_x)
    {
        offset_x = 1;
        target_x = 2;
    }
    else
    {
        offset_x = 0;
        target_x = 1;
    }

    if(_positive_offset_y)
    {
        offset_y = 1;
        target_y = 2;
    }
    else
    {
        offset_y = 0;
        target_y = 1;
    }

    if(_positive_offset_z)
    {
        offset_z = 1;
        target_z = 2;
    }
    else
    {
        offset_z = 0;
        target_z = 1;
    }

    for(unsigned int x = offset_x; x <= target_x; ++x)
    {
        for(unsigned int y = offset_y; y <= target_y; ++y)
        {
            for(unsigned int z = offset_z; z <= target_z; ++z)
            {
                if(_neighbors_data.values[x][y][z] == 0)
                    continue;

                glm::vec3 stride = {
                                    ((float)x - 1.0f) * m_cell_size,
                                    ((float)y - 1.0f) * m_cell_size,
                                    ((float)z - 1.0f) * m_cell_size
                                   };


                glm::vec3 voxel_center = _voxel_center + stride;
                glm::vec3 local_offset = voxel_center - _point;

                offset += local_offset;
                ++voxels_appended;
            }
        }
    }

    constexpr unsigned int Max_Ratio_Threshold = 2;
    constexpr unsigned int Max_Voxels_Around = 8;
    constexpr unsigned int MinMax_Difference = Max_Voxels_Around - Max_Ratio_Threshold;

    if(voxels_appended == 0)
        return;

    float ratio = 1.0f;
    if(voxels_appended > Max_Ratio_Threshold)
        ratio = 1.0f - ( (float)voxels_appended / (float)MinMax_Difference );

    offset *= ratio;
    offset /= (float)voxels_appended;
    _point += offset;
}

void Chunk_3D_Generation_Data::M_append_quad(const glm::vec3& _point_0, const glm::vec3& _point_1, const glm::vec3& _point_2, const glm::vec3& _point_3)
{
    m_points.push(_point_3);
    m_points.push(_point_0);
    m_points.push(_point_2);

    m_points.push(_point_2);
    m_points.push(_point_0);
    m_points.push(_point_1);
}

void Chunk_3D_Generation_Data::M_append_cell_data(const Neighbors_Data& _neighbors, unsigned int _x, unsigned int _y, unsigned int _z)
{
    Neighboring_Cells_Data neighbors_data = M_calculate_neighboring_cells_data(_neighbors, _x, _y, _z);

    glm::vec3 center = m_voxel->offset() + glm::vec3(
                                                     (float)_x * m_cell_size,
                                                     (float)_y * m_cell_size,
                                                     (float)_z * m_cell_size
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

    // M_balance_point(neighbors_data, center, left_down_near, false, false, false);
    // M_balance_point(neighbors_data, center, right_down_near, true, false, false);
    // M_balance_point(neighbors_data, center, left_down_far, false, false, true);
    // M_balance_point(neighbors_data, center, right_down_far, true, false, true);
    // M_balance_point(neighbors_data, center, left_up_near, false, true, false);
    // M_balance_point(neighbors_data, center, right_up_near, true, true, false);
    // M_balance_point(neighbors_data, center, left_up_far, false, true, true);
    // M_balance_point(neighbors_data, center, right_up_far, true, true, true);

    if(neighbors_data.values[0][1][1] == 0)
        M_append_quad(left_down_far, left_up_far, left_up_near, left_down_near);
    if(neighbors_data.values[2][1][1] == 0)
        M_append_quad(right_down_near, right_up_near, right_up_far, right_down_far);
    if(neighbors_data.values[1][0][1] == 0)
        M_append_quad(right_down_near, right_down_far, left_down_far, left_down_near);
    if(neighbors_data.values[1][2][1] == 0)
        M_append_quad(right_up_far, right_up_near, left_up_near, left_up_far);
    if(neighbors_data.values[1][1][0] == 0)
        M_append_quad(left_down_near, left_up_near, right_up_near, right_down_near);
    if(neighbors_data.values[1][1][2] == 0)
        M_append_quad(right_up_far, left_up_far, left_down_far, right_down_far);
}



void Chunk_3D_Generation_Data::extract_points(const Neighbors_Data& _neighbors)
{
    L_ASSERT(m_voxel);

    m_points.clear();

    for(unsigned int x = 0; x < m_grid.grid_size(); ++x)
    {
        for(unsigned int y = 0; y < m_grid.grid_size(); ++y)
        {
            for(unsigned int z = 0; z < m_grid.grid_size(); ++z)
            {
                if(m_grid.value_at(x, y, z) == 0)
                    continue;

                M_append_cell_data(_neighbors, x, y, z);
            }
        }
    }
}
