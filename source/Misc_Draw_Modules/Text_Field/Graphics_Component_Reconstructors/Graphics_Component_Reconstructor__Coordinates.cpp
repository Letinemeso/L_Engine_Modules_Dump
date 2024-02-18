#include <Misc_Draw_Modules/Text_Field/Graphics_Component_Reconstructors/Graphics_Component_Reconstructor__Coordinates.h>
#include <Misc_Draw_Modules/Text_Field/Draw_Module__Text_Field.h>

using namespace LMD;


glm::vec2 Graphics_Component_Reconstructor__Coordinates::M_calculate_raw_size(const Text_Field_Settings& _settings) const
{
    glm::vec2 result;

    if(_settings.text.size() == 0)
        return result;

    result.x = _settings.font->get_letter_data(_settings.text[0]).size_x;
    result.y = _settings.font->get_letter_data(_settings.text[0]).size_y;

    for(unsigned int i=1; i<_settings.text.size(); ++i)
    {
        result.x += _settings.font->get_letter_data(_settings.text[i]).size_x;
        float ch = _settings.font->get_letter_data(_settings.text[i]).size_y;
        if(ch > result.y)
            result.y = ch;
    }

    return result;
}

float Graphics_Component_Reconstructor__Coordinates::M_calculate_raw_scale(const Text_Field_Settings& _settings, const glm::vec2& _raw_size) const
{
    if(_settings.raw_size < 0.0f)
        return _settings.raw_size_multiplier;

    float result = 1.0f;

    if(_raw_size.x > _raw_size.y)
        result = _settings.raw_size / _raw_size.x;
    else
        result = _settings.raw_size / _raw_size.y;

    return result * _settings.raw_size_multiplier;
}



void Graphics_Component_Reconstructor__Coordinates::reconstruct_graphics_component()
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

    const unsigned int amount_per_symbol = m_graphics_component->buffer().floats_per_vertex() * vertices_per_symbol;
    const unsigned int amount = amount_per_symbol * settings.text.size();
    float* buffer = new float[amount]{0.0f};

    glm::vec2 raw_size = M_calculate_raw_size(settings);
    float raw_scale = M_calculate_raw_scale(settings, raw_size);

    float current_width = 0.0f;

    for(unsigned int i=0; i<settings.text.size(); ++i)
    {
        float* array = &(buffer[i * amount_per_symbol]);

        const LR::Letter_Data& letter_data = settings.font->get_letter_data(settings.text[i]);

        float scaled_size_x = letter_data.size_x * raw_scale;
        float scaled_size_y = letter_data.size_y * raw_scale;

        array[(fpv * 0)] = current_width;                       array[(fpv * 0) + 1] = scaled_size_y;
        array[(fpv * 1)] = current_width;                       array[(fpv * 1) + 1] = 0.0f;
        array[(fpv * 2)] = current_width + scaled_size_x;       array[(fpv * 2) + 1] = 0.0f;
        array[(fpv * 3)] = current_width;                       array[(fpv * 3) + 1] = scaled_size_y;
        array[(fpv * 4)] = current_width + scaled_size_x;       array[(fpv * 4) + 1] = 0.0f;
        array[(fpv * 5)] = current_width + scaled_size_x;       array[(fpv * 5) + 1] = scaled_size_y;

        current_width += scaled_size_x;
    }

    glm::vec3 raw_offset = settings.raw_offset;

    if(settings.horizontal_alignment == Text_Field_Settings::Horizontal_Alignment::Center)
        raw_offset.x -= raw_size.x / 2.0f;
    else if(settings.horizontal_alignment == Text_Field_Settings::Horizontal_Alignment::Right)
        raw_offset.x -= raw_size.x;

    if(settings.vertical_alignment == Text_Field_Settings::Vertical_Alignment::Center)
        raw_offset.y -= raw_size.y / 2.0f;
    else if(settings.vertical_alignment == Text_Field_Settings::Vertical_Alignment::Top)
        raw_offset.y -= raw_size.y;

    raw_offset *= raw_scale;

    const unsigned int max_coord_components = fpv < 3 ? fpv : 3;
    for(unsigned int vertex_i = 0; vertex_i < amount; vertex_i += fpv)
    {
        for(unsigned int i = 0; i < max_coord_components; ++i)
            buffer[vertex_i + i] += raw_offset[i];
    }

    m_graphics_component->buffer().use_array(buffer, amount);
}





LV::Variable_Base* Graphics_Component_Reconstructor_Stub__Coordinates::M_construct_product() const
{
    return new Graphics_Component_Reconstructor__Coordinates;
}

void Graphics_Component_Reconstructor_Stub__Coordinates::M_init_constructed_product(LV::Variable_Base *_product) const
{
    LR::Graphics_Component_Reconstructor_Stub::M_init_constructed_product(_product);

    Graphics_Component_Reconstructor__Coordinates* product = (Graphics_Component_Reconstructor__Coordinates*)_product;

}
