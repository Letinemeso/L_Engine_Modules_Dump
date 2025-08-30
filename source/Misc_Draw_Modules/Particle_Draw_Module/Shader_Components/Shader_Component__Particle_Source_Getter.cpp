#include <Misc_Draw_Modules/Particle_Draw_Module/Shader_Components/Shader_Component__Particle_Source_Getter.h>

#include <glew.h>

#include <Misc_Draw_Modules/Particle_Draw_Module/Draw_Module__Particle.h>

using namespace LMD;


void Shader_Component__Particle_Source_Getter::update(const LR::Draw_Module* _draw_module)
{
    L_ASSERT(LV::cast_variable<Draw_Module__Particle>(_draw_module));
    Draw_Module__Particle* module = (Draw_Module__Particle*)_draw_module;

    const glm::vec3& position = module->parent_transformation_data()->position();
    glUniform3f(m_module_position_uniform, position.x, position.y, position.z);
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Shader_Component_Stub__Particle_Source_Getter)

BUILDER_STUB_INITIALIZATION_FUNC(Shader_Component_Stub__Particle_Source_Getter)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
}
