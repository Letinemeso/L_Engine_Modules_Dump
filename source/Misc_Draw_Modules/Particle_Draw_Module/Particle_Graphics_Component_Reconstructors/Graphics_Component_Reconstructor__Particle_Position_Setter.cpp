#include <Misc_Draw_Modules/Particle_Draw_Module/Particle_Graphics_Component_Reconstructors/Graphics_Component_Reconstructor__Particle_Position_Setter.h>

#include <Math_Stuff.h>
#include <Object.h>

#include <Components/Graphics_Component.h>
#include <Draw_Modules/Draw_Module.h>

using namespace LMD;


void Graphics_Component_Reconstructor__Particle_Position_Setter::create_particle(unsigned int _particle_index)
{
    Parent_Type::create_particle(_particle_index);

    const LEti::Transformation_Data& transformation = m_draw_module->parent_object()->current_state();

    LR::Buffer::Element_Modification_Func modification_func = [&transformation, this](float& _element, unsigned int _index)
    {
        unsigned int component_index = _index % m_graphics_component->buffer().floats_per_vertex();
        _element += transformation.position()[component_index];
    };

    unsigned int offset = _particle_index * default_data().size();
    m_graphics_component->buffer().modify_buffer(modification_func, offset, default_data().size());
}

void Graphics_Component_Reconstructor__Particle_Position_Setter::destroy_particle(unsigned int _particle_index)
{
    Parent_Type::destroy_particle(_particle_index);
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Graphics_Component_Reconstructor_Stub__Particle_Position_Setter)

BUILDER_STUB_INITIALIZATION_FUNC(Graphics_Component_Reconstructor_Stub__Particle_Position_Setter)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;


}
