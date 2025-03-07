#include <Misc_Draw_Modules/Particle_Draw_Module/Particle_Graphics_Component_Reconstructors/Graphics_Component_Reconstructor__Particle.h>

#include <Components/Graphics_Component.h>
#include <Draw_Modules/Draw_Module.h>

using namespace LMD;


void Graphics_Component_Reconstructor__Particle::set_max_particles_amount(unsigned int _value)
{
    m_max_particles_amount = _value;

    unsigned int scaled_size = m_default_data.size() * m_max_particles_amount;
    m_graphics_component->buffer().resize(scaled_size);
}



void Graphics_Component_Reconstructor__Particle::create_particle(unsigned int _particle_index)
{
    unsigned int offset = _particle_index * m_default_data.size();
    m_draw_module->bind_vertex_array();
    m_graphics_component->buffer().copy_array(m_default_data.raw_data(), m_default_data.size(), offset);
}

void Graphics_Component_Reconstructor__Particle::destroy_particle(unsigned int _particle_index)
{

}



void Graphics_Component_Reconstructor__Particle::update(float _dt)
{

}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Graphics_Component_Reconstructor_Stub__Particle)

BUILDER_STUB_INITIALIZATION_FUNC(Graphics_Component_Reconstructor_Stub__Particle)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_default_data(default_data);
}
