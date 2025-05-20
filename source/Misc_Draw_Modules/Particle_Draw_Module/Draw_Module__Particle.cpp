#include <Misc_Draw_Modules/Particle_Draw_Module/Draw_Module__Particle.h>

#include <Math_Stuff.h>

#include <glew.h>

#include <Binds_Controller/Binds_Controller.h>

#include <Misc_Draw_Modules/Particle_Draw_Module/Particle_Graphics_Component_Reconstructors/Graphics_Component_Reconstructor__Particle.h>

using namespace LMD;


Draw_Module__Particle::Draw_Module__Particle()
{
    glGenBuffers(1, &m_element_array_buffer);
}

Draw_Module__Particle::~Draw_Module__Particle()
{
    glDeleteBuffers(1, &m_element_array_buffer);
}



void Draw_Module__Particle::set_max_particles(unsigned int _amount)
{
    m_particle_lifetimes.resize_and_fill(_amount, {});

    for(LR::Draw_Module::Graphics_Component_List::Const_Iterator it = graphics_components().begin(); !it.end_reached(); ++it)
    {
        LR::Graphics_Component__Default* component = *it;
        L_ASSERT(LV::cast_variable<Graphics_Component_Reconstructor__Particle>(component->reconstructor()));
        Graphics_Component_Reconstructor__Particle* reconstructor = (Graphics_Component_Reconstructor__Particle*)component->reconstructor();

        reconstructor->set_max_particles_amount(_amount);
    }

    recalculate_vertices_amount();
}


void Draw_Module__Particle::set_transformation_data(LEti::Transformation_Data* _data)
{
    Parent_Type::set_transformation_data(&m_initial_transformation);
    if(_data)
    {
        m_initial_transformation.set_rotation(_data->rotation());
        m_initial_transformation.set_scale(_data->scale());
    }
    m_parent_transformation_data = _data;
}

void Draw_Module__Particle::set_transformation_data_prev_state(const LEti::Transformation_Data* _data)
{
    Parent_Type::set_transformation_data_prev_state(&m_initial_transformation);
    m_parent_transformation_data_prev_state = _data;
}



void Draw_Module__Particle::M_update_emission_timer(float _dt)
{
    m_emission_timer.update(_dt);

    if( ! (!m_emission_timer.is_active() && m_emission_frequency > 0.000001f && !m_emission_is_paused))
        return;

    unsigned int ratio = _dt / m_emission_frequency;
    m_requested_particles_amount = 1;
    if(ratio > 0)
        m_requested_particles_amount = ratio;

    m_emission_timer.start(m_emission_frequency);
}


void Draw_Module__Particle::M_create_particle(unsigned int _index)
{
    float lifetime = LEti::Math::random_number_float(m_min_particles_lifetime, m_max_particles_lifetime);

    for(LR::Draw_Module::Graphics_Component_List::Const_Iterator it = graphics_components().begin(); !it.end_reached(); ++it)
    {
        LR::Graphics_Component__Default* component = *it;
        Graphics_Component_Reconstructor__Particle* reconstructor = (Graphics_Component_Reconstructor__Particle*)component->reconstructor();

        reconstructor->create_particle(_index);
    }

    m_particle_lifetimes[_index].start(lifetime);

    ++m_alive_particles_amount;
}

void Draw_Module__Particle::M_destroy_particle(unsigned int _index)
{
    for(LR::Draw_Module::Graphics_Component_List::Const_Iterator it = graphics_components().begin(); !it.end_reached(); ++it)
    {
        LR::Graphics_Component__Default* component = *it;
        Graphics_Component_Reconstructor__Particle* reconstructor = (Graphics_Component_Reconstructor__Particle*)component->reconstructor();

        reconstructor->destroy_particle(_index);
    }

    m_particle_lifetimes[_index].reset();

    --m_alive_particles_amount;
}


LDS::Vector<unsigned int> Draw_Module__Particle::M_calculate_active_vertices() const
{
    LDS::Vector<unsigned int> result(m_alive_particles_amount * m_vertices_per_particle);
    for(unsigned int i=0; i<m_particle_lifetimes.size(); ++i)
    {
        if(!m_particle_lifetimes[i].is_active())
            continue;

        unsigned int vertex_offset = i * m_vertices_per_particle;
        for(unsigned int stride = 0; stride < m_vertices_per_particle; ++stride)
            result.push(vertex_offset + stride);
    }
    return result;
}



void Draw_Module__Particle::emit_particles(unsigned int _amount)
{
    L_ASSERT(_amount > 0);

    for(unsigned int i=0; i<m_particle_lifetimes.size(); ++i)
    {
        if(m_particle_lifetimes[i].is_active())
            continue;

        M_create_particle(i);

        if(_amount == 0)
            break;

        --_amount;
    }
}



void Draw_Module__Particle::M_draw_internal() const
{
    if(m_alive_particles_amount == 0)
        return;

    //  idea for optimization: store uint array with size == total_particles_amount. when removing particle,
    //  replace its index with last index in array, decrease alive_particles_amount by 1
    LDS::Vector<unsigned int> active_vertices = M_calculate_active_vertices();

    LR::Binds_Controller::instance().bind_vertex_array(vertex_array());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_array_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, active_vertices.size() * sizeof(unsigned int), active_vertices.raw_data(), GL_STATIC_DRAW);

    glDrawElements(draw_mode(), active_vertices.size(), GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



void Draw_Module__Particle::update(float _dt)
{
    M_update_emission_timer(_dt);

    if(m_requested_particles_amount > 0)
    {
        emit_particles(m_requested_particles_amount);
        m_requested_particles_amount = 0;
    }

    for(unsigned int i=0; i<m_particle_lifetimes.size(); ++i)
    {
        LST::Timer& timer = m_particle_lifetimes[i];
        if(!timer.is_active())
            continue;

        timer.update(_dt);

        if(timer.is_active())
            continue;

        M_destroy_particle(i);
    }

    Parent_Type::update(_dt);
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Draw_Module_Stub__Particle)

BUILDER_STUB_INITIALIZATION_FUNC(Draw_Module_Stub__Particle)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_emission_frequency(emission_frequency);
    product->set_particles_lifetime(min_particles_lifetime, max_particles_lifetime);
    product->set_vertices_per_particle(M_calculate_vertices_per_particle());
    product->set_max_particles(max_particles_amount);
}



unsigned int Draw_Module_Stub__Particle::M_calculate_vertices_per_particle() const
{
    LV::Variable_Base::Childs_List::Const_Iterator it = graphics_component_stubs.begin();
    L_ASSERT(LV::cast_variable<LR::Graphics_Component_Stub__Default>(it->child_ptr));
    LR::Graphics_Component_Stub__Default* component_stub = (LR::Graphics_Component_Stub__Default*)(it->child_ptr);
    L_ASSERT(LV::cast_variable<Graphics_Component_Reconstructor_Stub__Particle>(component_stub->reconstructor_stub));
    Graphics_Component_Reconstructor_Stub__Particle* reconstructor_stub = (Graphics_Component_Reconstructor_Stub__Particle*)(component_stub->reconstructor_stub);

    unsigned int result = reconstructor_stub->default_data.size() / component_stub->floats_per_vertex;

    L_DEBUG_FUNC_NOARG([&]()
    {
        ++it;
        while(!it.end_reached())
        {
            L_ASSERT(LV::cast_variable<LR::Graphics_Component_Stub__Default>(it->child_ptr));
            component_stub = (LR::Graphics_Component_Stub__Default*)(it->child_ptr);
            L_ASSERT(LV::cast_variable<Graphics_Component_Reconstructor_Stub__Particle>(component_stub->reconstructor_stub));
            reconstructor_stub = (Graphics_Component_Reconstructor_Stub__Particle*)(component_stub->reconstructor_stub);

            unsigned int check_result = reconstructor_stub->default_data.size() / component_stub->floats_per_vertex;
            L_ASSERT(check_result == result);

            ++it;
        }
    });

    return result;
}
