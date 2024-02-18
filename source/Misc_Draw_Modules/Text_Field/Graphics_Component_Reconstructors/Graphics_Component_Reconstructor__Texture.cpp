#include <Misc_Draw_Modules/Text_Field/Graphics_Component_Reconstructors/Graphics_Component_Reconstructor__Texture.h>
#include <Misc_Draw_Modules/Text_Field/Draw_Module__Text_Field.h>

using namespace LMD;


void Graphics_Component_Reconstructor__Texture::reconstruct_graphics_component()
{
    L_ASSERT(LV::cast_variable<LMD::Draw_Module__Text_Field>(m_draw_module));
    L_ASSERT(LV::cast_variable<LR::Graphics_Component__Texture>(m_graphics_component));

    Draw_Module__Text_Field* tf = (Draw_Module__Text_Field*)m_draw_module;
    LR::Graphics_Component__Texture* component = (LR::Graphics_Component__Texture*)m_graphics_component;

    if(tf->settings() == tf->current_settings())
        return;

    const Text_Field_Settings& settings = tf->settings();

    if(component->get_picture() != settings.font->picture())
        component->set_picture(settings.font->picture());

    const unsigned int fpv = component->buffer().floats_per_vertex();
    L_ASSERT(fpv > 1);    //  single-dimentional font is not supported :D

    constexpr unsigned int vertices_per_symbol = 6;

    const unsigned int amount_per_symbol = component->buffer().floats_per_vertex() * vertices_per_symbol;
    const unsigned int amount = amount_per_symbol * settings.text.size();
    float* buffer = new float[amount]{0.0f};

    for(unsigned int i=0; i<settings.text.size(); ++i)
    {
        float* array = &(buffer[i * amount_per_symbol]);

        const LR::Letter_Data& letter_data = settings.font->get_letter_data(settings.text[i]);
        float letter_right = letter_data.pos_x + letter_data.size_x;
        float letter_top = letter_data.pos_y + letter_data.size_y;

        array[(fpv * 0)] = letter_data.pos_x;    array[(fpv * 0) + 1] = letter_top;
        array[(fpv * 1)] = letter_data.pos_x;    array[(fpv * 1) + 1] = letter_data.pos_y;
        array[(fpv * 2)] = letter_right;         array[(fpv * 2) + 1] = letter_data.pos_y;
        array[(fpv * 3)] = letter_data.pos_x;    array[(fpv * 3) + 1] = letter_top;
        array[(fpv * 4)] = letter_right;         array[(fpv * 4) + 1] = letter_data.pos_y;
        array[(fpv * 5)] = letter_right;         array[(fpv * 5) + 1] = letter_top;
    }

    component->buffer().use_array(buffer, amount);
    component->reconfigure_texture_coords();
}





LV::Variable_Base* Graphics_Component_Reconstructor_Stub__Texture::M_construct_product() const
{
    return new Graphics_Component_Reconstructor__Texture;
}

void Graphics_Component_Reconstructor_Stub__Texture::M_init_constructed_product(LV::Variable_Base *_product) const
{
    LR::Graphics_Component_Reconstructor_Stub::M_init_constructed_product(_product);

    Graphics_Component_Reconstructor__Texture* product = (Graphics_Component_Reconstructor__Texture*)_product;

}
