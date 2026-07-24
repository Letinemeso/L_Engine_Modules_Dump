#include <Tools/Voxel/Voxel_2D/Voxel_2D.h>

#include <string>

using namespace LGL;


Voxel_2D::Voxel_2D(Voxel_2D* _parent, unsigned int _max_depth, unsigned int _depth, unsigned int _id, float _position_x, float _position_y, float _size_x, float _size_y)
    : m_parent(_parent), m_max_depth(_max_depth), m_depth(_depth), m_id(_id), m_position_x(_position_x), m_position_y(_position_y), m_size_x(_size_x), m_size_y(_size_y)
{

}

Voxel_2D::~Voxel_2D()
{
    for(unsigned int i=0; i<4; ++i)
        delete m_childs[i];
}



Voxel_2D* Voxel_2D::construct_copy() const
{
    Voxel_2D* result = new Voxel_2D(nullptr, m_max_depth, m_depth, m_id, m_position_x, m_position_y, m_size_x, m_size_y);

    if(!is_split())
        return result;

    for(unsigned int i=0; i<4; ++i)
    {
        result->m_childs[i] = m_childs[i]->construct_copy();
        result->m_childs[i]->m_parent = result;
    }

    return result;
}



bool Voxel_2D::should_be_merged() const
{
    if(!is_split())
        return false;

    for(unsigned int i=0; i<4; ++i)
    {
        if(m_childs[i]->is_split())
            return false;
    }

    unsigned int first_child_id = m_childs[0]->id();
    for(unsigned int i=1; i<4; ++i)
    {
        if(m_childs[i]->id() != first_child_id)
            return false;
    }

    return true;
}

bool Voxel_2D::can_be_split() const
{
    return !reached_max_depth() && !is_split();
}



void Voxel_2D::M_init_childs()
{
    float size_x_halved = m_size_x * 0.5f;
    float size_y_halved = m_size_y * 0.5f;

    m_childs[0] = new Voxel_2D(this, m_max_depth, m_depth + 1, m_id, m_position_x, m_position_y, size_x_halved, size_y_halved);
    m_childs[1] = new Voxel_2D(this, m_max_depth, m_depth + 1, m_id, m_position_x + size_x_halved, m_position_y, size_x_halved, size_y_halved);
    m_childs[2] = new Voxel_2D(this, m_max_depth, m_depth + 1, m_id, m_position_x, m_position_y + size_y_halved, size_x_halved, size_y_halved);
    m_childs[3] = new Voxel_2D(this, m_max_depth, m_depth + 1, m_id, m_position_x + size_x_halved, m_position_y + size_y_halved, size_x_halved, size_y_halved);
}

void Voxel_2D::M_before_merge()
{

}



void Voxel_2D::split()
{
    L_ASSERT(!is_split());
    // L_ASSERT(can_be_split());    //  this probably shouldn't be here since voxel should be split'able forcefully (i guess)

    M_init_childs();
}

void Voxel_2D::merge()
{
    L_ASSERT(is_split());

    M_before_merge();

    for(unsigned int i=0; i<4; ++i)
    {
        delete m_childs[i];
        m_childs[i] = nullptr;
    }
}

void Voxel_2D::merge_subtrees_if_needed()
{
    if(!is_split())
        return;

    for(unsigned int i=0; i<4; ++i)
        m_childs[i]->merge_subtrees_if_needed();

    if(!should_be_merged())
        return;

    m_id = m_childs[0]->id();
    merge();
}



std::string Voxel_2D::M_parse_word(const std::string& _str, unsigned int _offset) const
{
    for(; _offset < _str.size(); ++_offset)
    {
        if(_str[_offset] != ' ')
            break;
    }

    if(_offset >= _str.size())
        return "";

    unsigned int word_size = 1;
    for(; _offset + word_size < _str.size(); ++word_size)
    {
        if(_str[_offset + word_size] == ' ')
            break;
    }

    return _str.substr(_offset, word_size);
}

std::string Voxel_2D::M_serialize_recursive() const
{
    std::string result = std::to_string(m_id) + ' ';
    if(is_split())
        result += "S ";
    else
        result += "M ";

    if(!is_split())
        return result;

    for(unsigned int i=0; i<4; ++i)
        result += child(i)->serialize_data();

    return result;
}

void Voxel_2D::M_deserialize_recursive(const std::string& _str, unsigned int& _offset)
{
    std::string id_str = M_parse_word(_str, _offset);
    L_ASSERT(id_str.size() > 0);
    _offset += id_str.size() + 1;
    std::string split_info = M_parse_word(_str, _offset);
    L_ASSERT(split_info == "S" || split_info == "M");
    _offset += 2;

    unsigned int id = std::stoi(id_str);
    bool is_split = (split_info == "S");

    m_id = id;

    if(!is_split)
        return;

    split();

    for(unsigned int i=0; i<4; ++i)
        m_childs[i]->M_deserialize_recursive(_str, _offset);
}



std::string Voxel_2D::serialize_data() const
{
    return M_serialize_recursive();
}

void Voxel_2D::deserialize_data(const std::string& _str)
{
    unsigned int offset = 0;
    M_deserialize_recursive(_str, offset);
}
