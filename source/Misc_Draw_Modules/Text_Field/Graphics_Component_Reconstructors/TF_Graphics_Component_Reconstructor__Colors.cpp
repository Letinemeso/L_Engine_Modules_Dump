#include <Misc_Draw_Modules/Text_Field/Graphics_Component_Reconstructors/TF_Graphics_Component_Reconstructor__Colors.h>
#include <Misc_Draw_Modules/Text_Field/Draw_Module__Text_Field.h>

using namespace LMD;


void TF_Graphics_Component_Reconstructor__Colors::update(float _dt)
{
    L_ASSERT(LV::cast_variable<LMD::Draw_Module__Text_Field>(m_draw_module));
    L_ASSERT(m_graphics_component);

    Draw_Module__Text_Field* tf = (Draw_Module__Text_Field*)m_draw_module;

    if(tf->settings() == tf->current_settings())
        return;

    const Text_Field_Settings& settings = tf->settings();

    const unsigned int fpv = m_graphics_component->buffer().floats_per_vertex();
    L_ASSERT(fpv > 1);    //  single-dimentional font is not supported :D

    constexpr unsigned int vertices_per_symbol = 6;

    unsigned int lines_amount = calculate_lines_amount(settings.text);

    const unsigned int amount_per_symbol = m_graphics_component->buffer().floats_per_vertex() * vertices_per_symbol;
    const unsigned int amount = amount_per_symbol * (settings.text.size() - (lines_amount - 1));
    float* buffer = new float[amount]{0.0f};

    for(unsigned int i=0; i<amount; ++i)
    {
        buffer[i] = 1.0f;
    }

    m_graphics_component->buffer().use_array(buffer, amount);
}





LV::Variable_Base* TF_Graphics_Component_Reconstructor_Stub__Colors::M_construct_product() const
{
    return new TF_Graphics_Component_Reconstructor__Colors;
}

void TF_Graphics_Component_Reconstructor_Stub__Colors::M_init_constructed_product(LV::Variable_Base *_product) const
{
    LR::Graphics_Component_Reconstructor_Stub::M_init_constructed_product(_product);

    TF_Graphics_Component_Reconstructor__Colors* product = (TF_Graphics_Component_Reconstructor__Colors*)_product;

}
