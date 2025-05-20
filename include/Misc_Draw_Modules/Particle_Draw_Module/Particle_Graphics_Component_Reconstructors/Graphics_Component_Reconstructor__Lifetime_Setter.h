#pragma once

#include <Data_Structures/Vector.h>
#include <Stuff/Timer.h>

#include <Components/Graphics_Component__Default.h>

#include <Misc_Draw_Modules/Particle_Draw_Module/Particle_Graphics_Component_Reconstructors/Graphics_Component_Reconstructor__Particle.h>
#include <Misc_Draw_Modules/Particle_Draw_Module/Draw_Module__Particle.h>


namespace LMD
{

class Graphics_Component_Reconstructor__Lifetime_Setter : public Graphics_Component_Reconstructor__Particle
{
public:
    INIT_VARIABLE(LMD::Graphics_Component_Reconstructor__Lifetime_Setter, LMD::Graphics_Component_Reconstructor__Particle)

private:
    using Timer_Vector = LDS::Vector<LST::Timer>;
    Timer_Vector m_timers;

private:
    Draw_Module__Particle* draw_module() const { L_ASSERT(LV::cast_variable<Draw_Module__Particle>(m_draw_module)); return (Draw_Module__Particle*)m_draw_module; }
    LR::Graphics_Component__Default* graphics_component() const { L_ASSERT(LV::cast_variable<LR::Graphics_Component__Default>(m_graphics_component)); return (LR::Graphics_Component__Default*)m_graphics_component; }

protected:
    void M_on_max_particles_amount_set() override;

public:
    void create_particle(unsigned int _particle_index) override;
    void destroy_particle(unsigned int _particle_index) override;

private:
    void M_destroy_particle_naturally(unsigned int _index);

public:
    void update(float _dt) override;

};


class Graphics_Component_Reconstructor_Stub__Lifetime_Setter : public LMD::Graphics_Component_Reconstructor_Stub__Particle
{
public:
    INIT_VARIABLE(LMD::Graphics_Component_Reconstructor_Stub__Lifetime_Setter, LMD::Graphics_Component_Reconstructor_Stub__Particle)

public:
    INIT_BUILDER_STUB(Graphics_Component_Reconstructor__Lifetime_Setter)

};

}
