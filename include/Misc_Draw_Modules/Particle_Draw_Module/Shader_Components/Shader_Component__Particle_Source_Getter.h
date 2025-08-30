#pragma once

#include <Shader/Shader_Components/Shader_Position_Getter_Component.h>


namespace LMD
{

    class Shader_Component__Particle_Source_Getter : public LR::Shader_Position_Getter_Component
    {
    public:
        INIT_VARIABLE(LMD::Shader_Component__Particle_Source_Getter, LR::Shader_Position_Getter_Component)

    public:
        void update(const LR::Draw_Module* _draw_module) override;

    };


    class Shader_Component_Stub__Particle_Source_Getter : public LR::Shader_Position_Getter_Component_Stub
    {
    public:
        INIT_VARIABLE(LMD::Shader_Component_Stub__Particle_Source_Getter, LR::Shader_Position_Getter_Component_Stub)

    public:
        INIT_BUILDER_STUB(Shader_Component__Particle_Source_Getter)

    };

}
