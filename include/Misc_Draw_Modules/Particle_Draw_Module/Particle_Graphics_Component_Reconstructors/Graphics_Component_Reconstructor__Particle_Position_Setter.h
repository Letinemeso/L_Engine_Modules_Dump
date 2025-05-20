#pragma once

#include <Components/Graphics_Component__Default.h>

#include <Misc_Draw_Modules/Particle_Draw_Module/Particle_Graphics_Component_Reconstructors/Graphics_Component_Reconstructor__Particle.h>


namespace LMD
{

    class Graphics_Component_Reconstructor__Particle_Position_Setter : public Graphics_Component_Reconstructor__Particle
    {
    public:
        INIT_VARIABLE(LMD::Graphics_Component_Reconstructor__Particle_Position_Setter, LMD::Graphics_Component_Reconstructor__Particle)

    private:
        LR::Graphics_Component__Default* graphics_component() const { L_ASSERT(LV::cast_variable<LR::Graphics_Component__Default>(m_graphics_component)); return (LR::Graphics_Component__Default*)m_graphics_component; }

    public:
        void create_particle(unsigned int _particle_index) override;
        void destroy_particle(unsigned int _particle_index) override;

    };


    class Graphics_Component_Reconstructor_Stub__Particle_Position_Setter : public LMD::Graphics_Component_Reconstructor_Stub__Particle
    {
    public:
        INIT_VARIABLE(LMD::Graphics_Component_Reconstructor_Stub__Particle_Position_Setter, LMD::Graphics_Component_Reconstructor_Stub__Particle)

    public:
        INIT_BUILDER_STUB(Graphics_Component_Reconstructor__Particle_Position_Setter)

    };

}
