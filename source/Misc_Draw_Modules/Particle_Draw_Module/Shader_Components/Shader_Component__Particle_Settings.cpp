#include <Misc_Draw_Modules/Particle_Draw_Module/Shader_Components/Shader_Component__Particle_Settings.h>

#include <glew.h>

#include <Misc_Draw_Modules/Particle_Draw_Module/Draw_Module__Particle.h>

#include <Stuff/String_With_Parameters.h>

using namespace LMD;


void Shader_Component__Particle_Settings::init(unsigned int _opengl_program_handle)
{
    Parent_Type::init(_opengl_program_handle);

    m_max_particles_uniform = M_extract_uniform_location_optional(m_max_particles_uniform_name);
}



void Shader_Component__Particle_Settings::update(const LR::Draw_Module* _draw_module)
{
    if(m_max_particles_uniform == -1)
        return;

    L_ASSERT(LV::cast_variable<Draw_Module__Particle>(_draw_module));
    Draw_Module__Particle* module = (Draw_Module__Particle*)_draw_module;

    glUniform1ui(m_max_particles_uniform, module->max_particles());
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Shader_Component_Stub__Particle_Settings)

BUILDER_STUB_INITIALIZATION_FUNC(Shader_Component_Stub__Particle_Settings)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    constexpr const char* Source =
        R"(

layout(local_size_x = {0}) in;

uniform uint {1};

uint current_particle_index()
{
    return gl_GlobalInvocationID.x;
}

    )";

    LST::String_With_Parameters str(Source);
    str.replace_parameter(0, std::to_string(invocations_amount))
        .replace_parameter(1, max_particles_uniform_name);

    product->set_source(str);

    product->set_max_particles_uniform_name(max_particles_uniform_name);
}
