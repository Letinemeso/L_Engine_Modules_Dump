#pragma once

#include <Shader/Shader_Components/Shader_Component.h>


namespace LMD
{

    class Shader_Component__Initialization_Counter : public LR::Shader_Component
    {
    public:
        INIT_VARIABLE(LMD::Shader_Component__Initialization_Counter, LR::Shader_Component)

    public:
        Dependencies get_dependencies() const override;

    };


    class Shader_Component_Stub__Initialization_Counter : public LR::Predefined_Shader_Component_Stub
    {
    public:
        INIT_VARIABLE(LMD::Shader_Component_Stub__Initialization_Counter, LR::Predefined_Shader_Component_Stub)

        INIT_FIELDS
        ADD_FIELD(unsigned int, counter_buffer_binding_index)
        ADD_FIELD(std::string, counter_name)
        ADD_FIELD(std::string, requested_particles_name)
        FIELDS_END

    public:
        unsigned int counter_buffer_binding_index = 15;
        std::string counter_name = "counter";
        std::string requested_particles_name = "requested_particles";

        //  shader expects LMD::Graphics_Component__Particles_Amount_Reader

        //  dependencies:
        //  - LMD::Shader_Component__Particle_Settings

        //  also, any other shader component MUST provide implementation for bool should_be_alive() function
        //  LMD::Shader_Component__Initialization_Time should have it

    public:
        INIT_BUILDER_STUB(Shader_Component__Initialization_Counter)

    };

}
