#pragma once

#include <Shader/Shader_Components/Shader_Component.h>


namespace LMD
{

    class Shader_Component_Stub__Update_Counter : public LR::Predefined_Shader_Component_Stub
    {
    public:
        INIT_VARIABLE(LMD::Shader_Component_Stub__Update_Counter, LR::Predefined_Shader_Component_Stub)

        INIT_FIELDS
        ADD_FIELD(unsigned int, counter_buffer_binding_index)
        ADD_FIELD(std::string, counter_name)
        FIELDS_END

    public:
        unsigned int counter_buffer_binding_index = 15;
        std::string counter_name = "counter";

        //  shader expects LMD::Graphics_Component__Particles_Amount_Reader

        //  any other shader component MUST provide implementation for bool just_born() and just_died() functions
        //  LMD::Shader_Component__Update_Time should have it

    public:
        INIT_BUILDER_STUB(LR::Shader_Component)

    };

}
