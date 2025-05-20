#include <Misc_Draw_Modules/Utility_Reconstructors/Graphics_Component_Reconstructor__Filler.h>

#include <Components/Graphics_Component.h>
#include <Components/Graphics_Component__Default.h>

using namespace LMD;


void Graphics_Component_Reconstructor__Filler::M_on_graphics_component_injected()
{
    L_ASSERT(LV::cast_variable<LR::Graphics_Component__Default>(m_graphics_component));
    LR::Graphics_Component__Default* graphics_component = (LR::Graphics_Component__Default*)m_graphics_component;

    unsigned int raw_buffer_size = m_copies_amount * m_data_per_copy.size();
    L_ASSERT(raw_buffer_size > 0);

    graphics_component->buffer().resize(raw_buffer_size);
    graphics_component->buffer().modify_buffer([this](float& _element, unsigned int)
    {
        float* elements_as_array = &_element;
        for(unsigned int i = 0; i < m_data_per_copy.size(); ++i)
            elements_as_array[i] = m_data_per_copy[i];
    }, 0, LR::Buffer::All_Elements, m_data_per_copy.size());
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Graphics_Component_Reconstructor_Stub__Filler)

BUILDER_STUB_INITIALIZATION_FUNC(Graphics_Component_Reconstructor_Stub__Filler)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_copies_amount(copies_amount);
    product->set_data_per_copy(data_per_copy);
}
