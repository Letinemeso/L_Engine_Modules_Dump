#include <Misc_Draw_Modules/Particle_Draw_Module/Shader_Components/Shader_Component__Update_Time_Component.h>

#include <glew.h>
#include <Shader/Shader_Components/Shader_Dt_Component.h>

#include <Stuff/String_With_Parameters.h>

using namespace LMD;


LR::Shader_Component::Dependencies Shader_Component__Update_Time_Component::get_dependencies() const
{
    LR::Shader_Component::Dependencies result = Parent_Type::get_dependencies();
    result.push(LR::Shader_Dt_Component::get_estimated_type());
    // result.push(LR::)

    return result;
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Shader_Component_Stub__Update_Time_Component)

BUILDER_STUB_INITIALIZATION_FUNC(Shader_Component_Stub__Update_Time_Component)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    constexpr const char* Source =
        R"(

layout(std430, binding = {0}) buffer Time_Buffer_{0}
{
    float {1}[];
};

uint current_time_buffer_offset_index()
{
    return current_particle_index() * 2;
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

bool just_born()
{
    uint index = current_time_buffer_offset_index();
    if (index >= {1}.length())
        return false;

    if({2} < 0.0001f)
        return false;

    if({1}[index + 1] <= 0.0f)
        return false;

    float time_before_update = {1}[index] + {2};
    return abs(time_before_update - {1}[index + 1]) < 0.0001f;
}

bool just_died()
{
    uint index = current_time_buffer_offset_index();
    if (index >= {1}.length())
        return false;

    if({2} < 0.0001f)
        return false;

    if({1}[index] > 0.0f)
        return false;

    float time_before_update = {1}[index] + {2};
    return time_before_update > 0.0f;
}

float lifetime_ratio()
{
    uint index = current_time_buffer_offset_index();
    if (index >= {1}.length())
        return 0.0f;

    if({1}[index] < 0.0f)
        return 0.0f;

    return {1}[index] / {1}[index + 1];
}

void {3}()
{
    uint index = current_time_buffer_offset_index();
    if (index >= {1}.length())
        return;

    {1}[index] -= {2};
}

    )";

    LST::String_With_Parameters str(Source);
    str.replace_parameter(0, std::to_string(time_buffer_binding_index))
        .replace_parameter(1, time_buffer_name)
        .replace_parameter(2, dt_variable_name)
        .replace_parameter(3, main_func_name);

    product->set_source(str);
    product->set_main_call(main_func_name + "();");
}
