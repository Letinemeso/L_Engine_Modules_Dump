#include <Misc_Draw_Modules/Particle_Draw_Module/Particle_Graphics_Component_Reconstructors/Graphics_Component_Reconstructor__Buffer_Resizer.h>

#include <Stuff/Math_Stuff.h>

#include <Components/Graphics_Component__Default.h>
#include <Draw_Modules/Draw_Module.h>

using namespace LMD;


void Graphics_Component_Reconstructor__Buffer_Resizer::update(float _dt)
{
    unsigned int default_data_size = m_default_data.size();
    L_ASSERT(default_data_size > 0);

    Draw_Module__Particle* module = draw_module();
    LR::Graphics_Component__Default* component = graphics_component();

    unsigned int scaled_size = default_data_size * module->max_particles();
    if(scaled_size == component->buffer().size())
        return;

    component->buffer().resize(scaled_size);
    component->buffer().modify_buffer( [this, default_data_size](float& _element, unsigned int _index)
    {
        float* array = &_element;
        for(unsigned int i = 0; i < default_data_size; ++i)
            array[i] = m_default_data[i];
    }, 0, LR::Buffer::All_Elements, default_data_size );
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Graphics_Component_Reconstructor_Stub__Buffer_Resizer)

BUILDER_STUB_INITIALIZATION_FUNC(Graphics_Component_Reconstructor_Stub__Buffer_Resizer)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_default_data(default_data);
}
