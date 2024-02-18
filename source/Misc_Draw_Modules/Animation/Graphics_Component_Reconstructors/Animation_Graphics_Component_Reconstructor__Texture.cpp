#include <Misc_Draw_Modules/Animation/Graphics_Component_Reconstructors/Animation_Graphics_Component_Reconstructor__Texture.h>
#include <Misc_Draw_Modules/Animation/Draw_Module__Animation.h>

using namespace LMD;


void Animation_Graphics_Component_Reconstructor__Texture::update(float _dt)
{
    L_ASSERT(LV::cast_variable<LMD::Draw_Module__Animation>(m_draw_module));
    L_ASSERT(LV::cast_variable<LR::Graphics_Component__Texture>(m_graphics_component));

    Draw_Module__Animation* tf = (Draw_Module__Animation*)m_draw_module;
    LR::Graphics_Component__Texture* component = (LR::Graphics_Component__Texture*)m_graphics_component;

}





LV::Variable_Base* Animation_Graphics_Component_Reconstructor_Stub__Texture::M_construct_product() const
{
    return new Animation_Graphics_Component_Reconstructor__Texture;
}

void Animation_Graphics_Component_Reconstructor_Stub__Texture::M_init_constructed_product(LV::Variable_Base *_product) const
{
    LR::Graphics_Component_Reconstructor_Stub::M_init_constructed_product(_product);

    Animation_Graphics_Component_Reconstructor__Texture* product = (Animation_Graphics_Component_Reconstructor__Texture*)_product;

}
