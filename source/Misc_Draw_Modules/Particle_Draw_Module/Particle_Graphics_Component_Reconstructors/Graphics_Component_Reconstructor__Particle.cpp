#include <Misc_Draw_Modules/Particle_Draw_Module/Particle_Graphics_Component_Reconstructors/Graphics_Component_Reconstructor__Particle.h>

#include <Components/Graphics_Component.h>
#include <Draw_Modules/Draw_Module.h>

using namespace LMD;


void Graphics_Component_Reconstructor__Particle::set_max_particles_amount(unsigned int _value)
{
    m_max_particles_amount = _value;

    unsigned int scaled_size = m_default_data.size() * m_max_particles_amount;
    float* scaled_data = new float[scaled_size];

    for(unsigned int i=0; i<m_max_particles_amount; ++i)
    {
        unsigned int scaled_data_offset = m_default_data.size() * i;

        for(unsigned int c=0; c<m_default_data.size(); ++c)
            scaled_data[scaled_data_offset + c] = m_default_data[c];
    }

    m_graphics_component->buffer().use_array(scaled_data, scaled_size);

    m_expected_lifetimes.resize_and_fill(m_max_particles_amount, 0.0f);
    m_remaining_lifetimes.resize_and_fill(m_max_particles_amount, 0.0f);
}



void Graphics_Component_Reconstructor__Particle::create_particle(unsigned int _particle_index, float _lifetime)
{
    m_expected_lifetimes[_particle_index] = _lifetime;
    m_remaining_lifetimes[_particle_index] = _lifetime;

    unsigned int offset = _particle_index * m_default_data.size();
    m_draw_module->bind_vertex_array();
    m_graphics_component->buffer().copy_array(m_default_data.raw_data(), m_default_data.size(), offset);
}

void Graphics_Component_Reconstructor__Particle::destroy_particle(unsigned int _particle_index)
{
    m_remaining_lifetimes[_particle_index] = 0.0f;
}



void Graphics_Component_Reconstructor__Particle::update(float _dt)
{
    for(unsigned int i=0; i<m_max_particles_amount; ++i)
    {
        if(m_remaining_lifetimes[i] <= 0.0f)
            continue;

        m_remaining_lifetimes[i] -= _dt;
    }
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Graphics_Component_Reconstructor_Stub__Particle)

BUILDER_STUB_INITIALIZATION_FUNC(Graphics_Component_Reconstructor_Stub__Particle)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_default_data(default_data);
}
