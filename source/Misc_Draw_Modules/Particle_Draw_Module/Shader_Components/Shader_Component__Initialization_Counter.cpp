#include <Misc_Draw_Modules/Particle_Draw_Module/Shader_Components/Shader_Component__Initialization_Counter.h>

#include <glew.h>

#include <Stuff/String_With_Parameters.h>

#include <Misc_Draw_Modules/Particle_Draw_Module/Shader_Components/Shader_Component__Particle_Settings.h>

using namespace LMD;


LR::Shader_Component::Dependencies Shader_Component__Initialization_Counter::get_dependencies() const
{
    LR::Shader_Component::Dependencies result = Parent_Type::get_dependencies();
    result.push(Shader_Component__Particle_Settings::get_estimated_type());
    return result;
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Shader_Component_Stub__Initialization_Counter)

BUILDER_STUB_INITIALIZATION_FUNC(Shader_Component_Stub__Initialization_Counter)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    constexpr const char* Source =
    R"(

layout(std430, binding = {0}) buffer counter_buffer
{
    uint {1};
    int {2};
};


bool should_be_alive();

bool should_create_particle()
{
    if(current_particle_index() >= max_particles)
        return false;

    if(should_be_alive())
        return false;

    int requested_decreased = atomicAdd({2}, -1);
    if(requested_decreased <= 0)
        return false;

    return true;
}

    )";

    LST::String_With_Parameters str(Source);
    str.replace_parameter(0, std::to_string(counter_buffer_binding_index))
        .replace_parameter(1, counter_name)
        .replace_parameter(2, requested_particles_name);

    product->set_source(str);
    product->set_main_call(
        R"(
            if(!should_create_particle())
                return;
        )");
}
