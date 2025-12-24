#include <Misc_Draw_Modules/Particle_Draw_Module/Shader_Components/Shader_Component__Update_Counter.h>

#include <glew.h>

#include <Stuff/String_With_Parameters.h>

#include <Misc_Draw_Modules/Particle_Draw_Module/Shader_Components/Shader_Component__Particle_Settings.h>

using namespace LMD;


BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Shader_Component_Stub__Update_Counter)

BUILDER_STUB_INITIALIZATION_FUNC(Shader_Component_Stub__Update_Counter)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    constexpr const char* Source =
        R"(

layout(std430, binding = {0}) buffer counter_buffer
{
    uint {1};
};

bool just_born();
bool just_died();

void process_counter_calculation()
{
    if(just_born())
        atomicAdd({1}, 1);
    else if(just_died())
        atomicAdd({1}, -1);
}

    )";

    LST::String_With_Parameters str(Source);
    str.replace_parameter(0, std::to_string(counter_buffer_binding_index))
        .replace_parameter(1, counter_name);

    product->set_source(str);
    product->set_main_call("process_counter_calculation();");
}
