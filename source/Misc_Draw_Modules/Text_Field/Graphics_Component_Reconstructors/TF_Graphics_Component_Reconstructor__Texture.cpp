#include <Misc_Draw_Modules/Text_Field/Graphics_Component_Reconstructors/TF_Graphics_Component_Reconstructor__Texture.h>
#include <Misc_Draw_Modules/Text_Field/Draw_Module__Text_Field.h>

using namespace LMD;


void TF_Graphics_Component_Reconstructor__Texture::update(float /*_dt*/)
{
    L_ASSERT(LV::cast_variable<LMD::Draw_Module__Text_Field>(m_draw_module));
    L_ASSERT(LV::cast_variable<LR::Graphics_Component__Texture>(m_graphics_component));

    Draw_Module__Text_Field* tf = (Draw_Module__Text_Field*)m_draw_module;
    LR::Graphics_Component__Texture* component = (LR::Graphics_Component__Texture*)m_graphics_component;

    if(tf->settings() == tf->current_settings())
        return;

    const Text_Field_Settings& settings = tf->settings();

    if(component->get_picture() != settings.picture)
        component->set_picture(settings.picture);

    const unsigned int fpv = component->buffer().floats_per_vertex();
    L_ASSERT(fpv > 1);    //  single-dimentional font is not supported :D

    constexpr unsigned int vertices_per_symbol = 6;

    unsigned int lines_amount = calculate_lines_amount(settings.text);

    const unsigned int amount_per_symbol = component->buffer().floats_per_vertex() * vertices_per_symbol;
    const unsigned int amount = amount_per_symbol * (settings.text.size() - (lines_amount - 1));
    float* buffer = new float[amount]{0.0f};

    unsigned int newline_symbols_met = 0;

    for(unsigned int i=0; i<settings.text.size(); ++i)
    {
        unsigned int offset_if_newline = offset_if_new_line(settings.text, i);

        if(offset_if_newline > 0)
        {
            ++newline_symbols_met;
            i += offset_if_newline - 1;
            continue;
        }

        float* array = &(buffer[(i - newline_symbols_met) * amount_per_symbol]);

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

    component->buffer().resize(amount);
    component->buffer().copy_array(buffer, amount);
    component->reconfigure_texture_coords();

    delete[] buffer;
}





LV::Variable_Base* TF_Graphics_Component_Reconstructor_Stub__Texture::M_construct_product() const
{
    return new TF_Graphics_Component_Reconstructor__Texture;
}

void TF_Graphics_Component_Reconstructor_Stub__Texture::M_init_constructed_product(LV::Variable_Base *_product) const
{
    LR::Graphics_Component_Reconstructor_Stub::M_init_constructed_product(_product);

    TF_Graphics_Component_Reconstructor__Texture* product = (TF_Graphics_Component_Reconstructor__Texture*)_product;

}
