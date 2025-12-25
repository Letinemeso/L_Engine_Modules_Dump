#include <Misc_Draw_Modules/Particle_Draw_Module/Shader_Components/Shader_Component__Initialization_Time.h>

#include <glew.h>
#include <Shader/Shader_Components/Shader_Dt_Component.h>

#include <Stuff/String_With_Parameters.h>

#include <Misc_Draw_Modules/Particle_Draw_Module/Shader_Components/Shader_Component__Particle_Settings.h>

using namespace LMD;


LR::Shader_Component::Dependencies Shader_Component__Initialization_Time::get_dependencies() const
{
    LR::Shader_Component::Dependencies result = Parent_Type::get_dependencies();
    result.push(LMD::Shader_Component__Particle_Settings::get_estimated_type());
    return result;
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Shader_Component_Stub__Initialization_Time)

BUILDER_STUB_INITIALIZATION_FUNC(Shader_Component_Stub__Initialization_Time)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    constexpr const char* Source =
        R"(

layout(std430, binding = {0}) buffer time_buffer
{
    float {1}[];
};

uint current_time_buffer_offset_index()
{
    return current_particle_index() * 2;
}

float get_remaining_time()
{
    uint index = current_time_buffer_offset_index();
    if (index >= {1}.length())
        return -1.0f;

    return {1}[index];
}

float get_expected_time()
{
    uint index = current_time_buffer_offset_index() + 1;
    if (index >= {1}.length())
        return -1.0f;

    return {1}[index];
}

float calculate_lifetime_ratio()
{
    return get_remaining_time() / get_expected_time();
}

void set_remaining_time(float _value)
{
    uint index = current_time_buffer_offset_index();
    if (index >= {1}.length())
        return;

    {1}[index] = _value;
}

void set_expected_time(float _value)
{
    uint index = current_time_buffer_offset_index() + 1;
    if (index >= {1}.length())
        return;

    {1}[index] = _value;
}

bool should_be_alive()
{
    uint index = current_time_buffer_offset_index();
    if (index >= {1}.length())
        return false;

    if({1}[index] > 0.0f)
        return true;
    return false;
}

    )";

    LST::String_With_Parameters str(Source);
    str.replace_parameter(0, std::to_string(time_buffer_binding_index))
        .replace_parameter(1, time_buffer_name);

    product->set_source(str);
}
