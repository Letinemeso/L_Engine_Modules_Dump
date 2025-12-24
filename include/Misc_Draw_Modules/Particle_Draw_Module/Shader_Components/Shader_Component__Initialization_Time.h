#pragma once

#include <Shader/Shader_Components/Shader_Component.h>


namespace LMD
{

    class Shader_Component__Initialization_Time : public LR::Shader_Component
    {
    public:
        INIT_VARIABLE(LMD::Shader_Component__Initialization_Time, LR::Shader_Component)

    public:
        Dependencies get_dependencies() const override;

    };


    class Shader_Component_Stub__Initialization_Time : public LR::Predefined_Shader_Component_Stub
    {
    public:
        INIT_VARIABLE(LMD::Shader_Component_Stub__Initialization_Time, LR::Predefined_Shader_Component_Stub)

        INIT_FIELDS
        ADD_FIELD(unsigned int, time_buffer_binding_index)
        ADD_FIELD(std::string, time_buffer_name)
        FIELDS_END

    public:
        unsigned int time_buffer_binding_index = 0;
        std::string time_buffer_name = "time_data";

        //  buffer is expected to contain 2 floats per particle: { remaining_lifetime, total_lifetime }

        //  dependencies:
        //  - LMD::Shader_Component__Particle_Settings

    public:
        INIT_BUILDER_STUB(Shader_Component__Initialization_Time)

    };

}
