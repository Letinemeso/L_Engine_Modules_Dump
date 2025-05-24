#include <Misc_Draw_Modules/Particle_Draw_Module/Particle_Graphics_Component_Reconstructors/Graphics_Component_Reconstructor__Lifetime_Setter.h>

#include <Math_Stuff.h>
#include <Object.h>

#include <Draw_Modules/Draw_Module.h>

using namespace LMD;


void Graphics_Component_Reconstructor__Lifetime_Setter::M_on_max_particles_amount_set()
{
    m_timers.resize_and_fill(max_particles_amount(), {});
}



void Graphics_Component_Reconstructor__Lifetime_Setter::create_particle(unsigned int _particle_index)
{
    Parent_Type::create_particle(_particle_index);

    LR::Graphics_Component__Default* component = graphics_component();

    float generated_lifetime = 0.0f;
    component->buffer().modify_buffer([&](float& _element, unsigned int _index)
    {
        generated_lifetime = _element;
    }, _particle_index * default_data().size(), 1);

    m_timers[_particle_index].start(generated_lifetime);
}

void Graphics_Component_Reconstructor__Lifetime_Setter::destroy_particle(unsigned int _particle_index)
{
    Parent_Type::destroy_particle(_particle_index);

    m_timers[_particle_index].reset();
}



void Graphics_Component_Reconstructor__Lifetime_Setter::M_destroy_particle_naturally(unsigned int _index)
{
    Draw_Module__Particle* module = draw_module();
    // module->destroy_particle(_index);
}



void Graphics_Component_Reconstructor__Lifetime_Setter::update(float _dt)
{
    for(unsigned int i = 0; i < m_timers.size(); ++i)
    {
        LST::Timer& timer = m_timers[i];

        if(!timer.is_active())
            continue;

        timer.update(_dt);

        if(!timer.is_active())
            M_destroy_particle_naturally(i);
    }
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Graphics_Component_Reconstructor_Stub__Lifetime_Setter)

BUILDER_STUB_INITIALIZATION_FUNC(Graphics_Component_Reconstructor_Stub__Lifetime_Setter)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;


}
