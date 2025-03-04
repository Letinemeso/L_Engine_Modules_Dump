#include <Misc_Draw_Modules/Particle_Draw_Module/Particle_Graphics_Component_Reconstructors/Graphics_Component_Reconstructor__Particle.h>

#include <Components/Graphics_Component.h>

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
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Graphics_Component_Reconstructor_Stub__Particle)

BUILDER_STUB_INITIALIZATION_FUNC(Graphics_Component_Reconstructor_Stub__Particle)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_default_data(default_data);
}
