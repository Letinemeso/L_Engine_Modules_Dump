#include <Tools/Voxel/Voxel_2D/Voxel_2D_Controller.h>

#include <cmath>

using namespace LGL;


Voxel_2D_Controller::Voxel_2D_Controller()
{

}

Voxel_2D_Controller::~Voxel_2D_Controller()
{
    save_voxels();

    for(Voxel_List::Iterator it = m_voxels.begin(); !it.end_reached(); ++it)
    {
        Voxel_Data& voxel_data = *it;
        delete voxel_data.voxel;
    }

    delete m_generator;
    delete m_serializer;
}



typename Voxel_2D_Controller::Voxel_List::Iterator Voxel_2D_Controller::M_find_voxel(const Voxel_2D* _voxel)
{
    for(Voxel_List::Iterator it = m_voxels.begin(); !it.end_reached(); ++it)
    {
        Voxel_Data& voxel_data = *it;

        if(voxel_data.voxel == _voxel)
            return it;
    }
    return {};
}

typename Voxel_2D_Controller::Voxel_List::Iterator Voxel_2D_Controller::M_find_voxel(int _index_x, int _index_y)
{
    for(Voxel_List::Iterator it = m_voxels.begin(); !it.end_reached(); ++it)
    {
        Voxel_Data& voxel_data = *it;

        if(voxel_data.index_x == _index_x && voxel_data.index_y == _index_y)
            return it;
    }
    return {};
}

int Voxel_2D_Controller::M_calculate_world_center_index(float _world_center, float _voxel_size) const
{
    int result = _world_center / _voxel_size;
    if(_world_center < 0.0f)
        result -= 1;
    return result;
}



void Voxel_2D_Controller::remove_all_voxels()
{
    for(Voxel_List::Iterator it = m_voxels.begin(); !it.end_reached(); ++it)
    {
        Voxel_Data& voxel_data = *it;

        if(m_on_voxel_removed)
            m_on_voxel_removed(voxel_data);

        delete voxel_data.voxel;
    }
    m_voxels.clear();
}

void Voxel_2D_Controller::reload_voxels(float _world_center_x, float _world_center_y)
{
    m_current_world_center_x = M_calculate_world_center_index(_world_center_x, m_expected_voxel_size_x);
    m_current_world_center_y = M_calculate_world_center_index(_world_center_y, m_expected_voxel_size_y);

    reload_voxels(true);
}

void Voxel_2D_Controller::reload_voxels(bool _should_save)
{
    if(_should_save)
        save_voxels();

    remove_all_voxels();

    int voxels_from_center = m_loaded_voxels_amount_from_center;

    for(int x = -voxels_from_center; x < voxels_from_center; ++x)
    {
        for(int y = -voxels_from_center; y < voxels_from_center; ++y)
        {
            int index_x = m_current_world_center_x + x;
            int index_y = m_current_world_center_y + y;

            Voxel_2D* voxel = nullptr;
            if(m_serializer)
                voxel = m_serializer->load_voxel(index_x, index_y);

            if(!voxel)
            {
                if(!m_generator)
                    continue;
                voxel = m_generator->generate(x, y, m_expected_max_depth, m_expected_voxel_size_x, m_expected_voxel_size_y);
            }

            L_ASSERT(voxel);

            Voxel_Data voxel_data;
            voxel_data.index_x = index_x;
            voxel_data.index_y = index_y;
            voxel_data.voxel = voxel;
            voxel_data.changes_were_made = true;

            if(m_on_voxel_added)
                m_on_voxel_added(voxel_data);

            m_voxels.push_back(voxel_data);
        }
    }

    for(Voxel_List::Iterator it = m_voxels.begin(); !it.end_reached(); ++it)
    {
        Voxel_2D* voxel = it->voxel;
        voxel->merge_subtrees_if_needed();
    }

    if(m_on_world_center_changed)
        m_on_world_center_changed();
}

void Voxel_2D_Controller::save_voxels()
{
    if(!m_serializer)
        return;

    for(Voxel_List::Iterator it = m_voxels.begin(); !it.end_reached(); ++it)
    {
        Voxel_Data& voxel_data = *it;

        m_serializer->save_voxel(voxel_data.voxel, voxel_data.index_x, voxel_data.index_y);
    }
}

void Voxel_2D_Controller::update_world_center(float _world_center_x, float _world_center_y)
{
    int center_index_x = M_calculate_world_center_index(_world_center_x, m_expected_voxel_size_x);
    int center_index_y = M_calculate_world_center_index(_world_center_y, m_expected_voxel_size_y);

    if(center_index_x == m_current_world_center_x && center_index_y == m_current_world_center_y && m_voxels.size() > 0)
        return;

    int voxels_from_center = m_loaded_voxels_amount_from_center;

    int left_voxel_index = center_index_x - voxels_from_center;
    int right_voxel_index = center_index_x + voxels_from_center;
    int bottom_voxel_index = center_index_y - voxels_from_center;
    int top_voxel_index = center_index_y + voxels_from_center;

    Voxel_List::Iterator it = m_voxels.begin();
    while(!it.end_reached())                        //  if constantly searching through the list will be too much, there's an option to calculate
    {                                               //  current voxel boundaries and use them later to identify the need to load another voxel
        Voxel_Data& voxel_data = *it;

        if(voxel_data.index_x >= left_voxel_index && voxel_data.index_x < right_voxel_index && voxel_data.index_y >= bottom_voxel_index && voxel_data.index_y < top_voxel_index)
        {
            ++it;
            continue;
        }

        if(m_serializer)
            m_serializer->save_voxel(voxel_data.voxel, voxel_data.index_x, voxel_data.index_y);

        if(m_on_voxel_removed)
            m_on_voxel_removed(voxel_data);

        delete voxel_data.voxel;
        it = m_voxels.erase_and_iterate_forward(it);
    }

    for(int x = left_voxel_index; x < right_voxel_index; ++x)
    {
        for(int y = bottom_voxel_index; y < top_voxel_index; ++y)
        {
            Voxel_List::Iterator maybe_voxel_it = M_find_voxel(x, y);
            if(maybe_voxel_it.is_ok())
                continue;

            Voxel_2D* voxel = nullptr;
            if(m_serializer)
                voxel = m_serializer->load_voxel(x, y);

            if(!voxel)
            {
                if(!m_generator)
                    continue;
                voxel = m_generator->generate(x, y, m_expected_max_depth, m_expected_voxel_size_x, m_expected_voxel_size_y);
            }

            L_ASSERT(voxel);

            Voxel_Data voxel_data;
            voxel_data.index_x = x;
            voxel_data.index_y = y;
            voxel_data.voxel = voxel;
            voxel_data.changes_were_made = true;

            if(m_on_voxel_added)
                m_on_voxel_added(voxel_data);

            m_voxels.push_back(voxel_data);
        }
    }

    m_current_world_center_x = center_index_x;
    m_current_world_center_y = center_index_y;

    for(Voxel_List::Iterator it = m_voxels.begin(); !it.end_reached(); ++it)
    {
        Voxel_2D* voxel = it->voxel;
        voxel->merge_subtrees_if_needed();
    }

    if(m_on_world_center_changed)
        m_on_world_center_changed();
}



void Voxel_2D_Controller::insert_voxel(Voxel_2D* _voxel)
{
    L_ASSERT(!M_find_voxel(_voxel).is_ok());
    L_ASSERT(fabs(m_expected_voxel_size_x - _voxel->size_x()) < 0.0001f);
    L_ASSERT(fabs(m_expected_voxel_size_y - _voxel->size_y()) < 0.0001f);
    L_ASSERT(m_expected_max_depth == _voxel->max_depth());

    Voxel_Data voxel_data;

    voxel_data.index_x = _voxel->position_x() / m_expected_voxel_size_x;
    voxel_data.index_y = _voxel->position_y() / m_expected_voxel_size_y;
    L_ASSERT(!M_find_voxel(voxel_data.index_x, voxel_data.index_y).is_ok());
    voxel_data.voxel = _voxel;
    voxel_data.changes_were_made = true;

    if(m_on_voxel_added)
        m_on_voxel_added(voxel_data);

    m_voxels.push_back(voxel_data);
}

void Voxel_2D_Controller::remove_voxel(Voxel_2D* _voxel)
{
    Voxel_List::Iterator voxel_it = M_find_voxel(_voxel);
    L_ASSERT(voxel_it.is_ok());

    if(m_on_voxel_removed)
        m_on_voxel_removed(*voxel_it);

    delete _voxel;
    m_voxels.erase(voxel_it);
}

void Voxel_2D_Controller::remove_voxel(int _index_x, int _index_y)
{
    Voxel_List::Iterator voxel_it = M_find_voxel(_index_x, _index_y);
    L_ASSERT(voxel_it.is_ok());

    Voxel_Data& voxel_data = *voxel_it;

    if(m_on_voxel_removed)
        m_on_voxel_removed(voxel_data);

    delete voxel_data.voxel;
    m_voxels.erase(voxel_it);
}

void Voxel_2D_Controller::reload_voxel_if_registered(int _index_x, int _index_y)
{
    Voxel_List::Iterator voxel_it = M_find_voxel(_index_x, _index_y);
    if(!voxel_it.is_ok())
        return;

    Voxel_Data& voxel_data = *voxel_it;

    if(m_on_voxel_removed)
        m_on_voxel_removed(voxel_data);

    delete voxel_data.voxel;
    voxel_data.voxel = nullptr;

    if(m_serializer)
        voxel_data.voxel = m_serializer->load_voxel(_index_x, _index_y);
    else if(m_generator)
        voxel_data.voxel = m_generator->generate(_index_x, _index_y, m_expected_max_depth, m_expected_voxel_size_x, m_expected_voxel_size_y);

    L_ASSERT(voxel_data.voxel);

    if(m_on_voxel_added)
        m_on_voxel_added(voxel_data);

    voxel_data.changes_were_made = true;
}



void Voxel_2D_Controller::process_logic_for_voxels(const Voxel_Modification_Func& _voxel_modification_func)
{
    L_ASSERT(_voxel_modification_func);

    for(Voxel_List::Iterator it = m_voxels.begin(); !it.end_reached(); ++it)
    {
        bool changes_were_made = _voxel_modification_func(it->voxel);
        if(!changes_were_made)
            continue;

        it->changes_were_made = true;
        it->voxel->merge_subtrees_if_needed();
    }
}



void Voxel_2D_Controller::mark_all_unchanged()
{
    for(Voxel_List::Iterator it = m_voxels.begin(); !it.end_reached(); ++it)
        it->changes_were_made = false;
}
