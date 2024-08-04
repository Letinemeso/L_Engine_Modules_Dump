#include <Misc_Draw_Modules/Text_Field/Graphics_Component_Reconstructors/TF_Graphics_Component_Reconstructor__Coordinates.h>
#include <Misc_Draw_Modules/Text_Field/Draw_Module__Text_Field.h>

using namespace LMD;


float TF_Graphics_Component_Reconstructor__Coordinates::M_calculate_next_line_offset_x(const std::string& _text, unsigned int _offset, const glm::vec2& _raw_size, float _raw_scale, const Text_Field_Settings& _settings) const
{
    if(_offset >= _text.size())
        return 0.0f;

    if(_settings.horizontal_alignment == Text_Field_Settings::Horizontal_Alignment::Left)
        return 0.0f;

    float line_size = 0.0f;
    for(unsigned int i=_offset; i < _text.size(); ++i)
    {
        if(offset_if_new_line(_text, i))
            break;

        const LR::Letter_Data& letter = _settings.font->get_letter_data(_text[i]);
        line_size += letter.size_x;
    }

    line_size *= _raw_scale;

    float result = _raw_size.x - line_size;

    if(_settings.horizontal_alignment == Text_Field_Settings::Horizontal_Alignment::Center)
        result *= 0.5f;

    return result;
}

float TF_Graphics_Component_Reconstructor__Coordinates::M_get_highest_letter_size(const Text_Field_Settings& _settings) const
{
    const LR::Font* font = _settings.font;
    const LR::Letter_Data* letters = font->letters();

    float result = 0.0f;

    for(unsigned int i=0; i<font->letters_amount(); ++i)
    {
        const LR::Letter_Data& letter = letters[i];

        if(letter.letter == 0)
            continue;

        if(letter.size_y > result)
            result = letter.size_y;
    }

    return result;
}

glm::vec2 TF_Graphics_Component_Reconstructor__Coordinates::M_calculate_raw_size(const Text_Field_Settings& _settings) const
{
    glm::vec2 result;

    const std::string& text = _settings.text;
    const LR::Font* font = _settings.font;

    if(text.size() == 0)
        return result;

    result.x = 0.0f;
    result.y = m_highest_letter;

    float widest_line = 0.0f;

    for(unsigned int i=0; i<text.size(); ++i)
    {
        unsigned int offset_if_newline = offset_if_new_line(text, i);

        if(offset_if_newline == 0)
        {
            const LR::Letter_Data& letter = font->get_letter_data(text[i]);
            widest_line += letter.size_x;

            continue;
        }

        result.y += m_highest_letter;

        if(widest_line > result.x)
            result.x = widest_line;

        widest_line = 0.0f;

        i += offset_if_newline - 1;
    }

    if(widest_line > result.x)
        result.x = widest_line;

    return result;
}

float TF_Graphics_Component_Reconstructor__Coordinates::M_calculate_raw_scale(const Text_Field_Settings& _settings, const glm::vec2& _raw_size) const
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



void TF_Graphics_Component_Reconstructor__Coordinates::update(float _dt)
{
    L_ASSERT(LV::cast_variable<LMD::Draw_Module__Text_Field>(m_draw_module));
    L_ASSERT(m_graphics_component);

    Draw_Module__Text_Field* tf = (Draw_Module__Text_Field*)m_draw_module;

    const Text_Field_Settings& settings = tf->settings();

    if(settings == tf->current_settings())
        return;

    if(settings.font != tf->current_settings().font)
        m_highest_letter = M_get_highest_letter_size(settings);

    const unsigned int fpv = m_graphics_component->buffer().floats_per_vertex();
    L_ASSERT(fpv > 1);    //  single-dimentional font is not supported :D

    constexpr unsigned int vertices_per_symbol = 6;

    unsigned int lines_amount = calculate_lines_amount(settings.text);

    const unsigned int amount_per_symbol = m_graphics_component->buffer().floats_per_vertex() * vertices_per_symbol;
    const unsigned int amount = amount_per_symbol * (settings.text.size() - (lines_amount - 1));

    float* buffer = new float[amount]{0.0f};

    glm::vec2 raw_size = M_calculate_raw_size(settings);
    float raw_scale = M_calculate_raw_scale(settings, raw_size);

    float offset_per_line = m_highest_letter * raw_scale;
    float current_offset_x = M_calculate_next_line_offset_x(settings.text, 0, raw_size, raw_scale, settings);
    float current_offset_y = offset_per_line * (float)(lines_amount - 1);

    unsigned int newline_symbols_met = 0;

    for(unsigned int i=0; i<settings.text.size(); ++i)
    {
        unsigned int offset_if_newline = offset_if_new_line(settings.text, i);

        if(offset_if_newline > 0)
        {
            ++newline_symbols_met;
            current_offset_x = M_calculate_next_line_offset_x(settings.text, i + offset_if_newline, raw_size, raw_scale, settings);
            current_offset_y -= offset_per_line;
            i += offset_if_newline - 1;
            continue;
        }

        float* array = &(buffer[(i - newline_symbols_met) * amount_per_symbol]);

        const LR::Letter_Data& letter_data = settings.font->get_letter_data(settings.text[i]);

        float scaled_size_x = letter_data.size_x * raw_scale;
        float scaled_size_y = letter_data.size_y * raw_scale;

        array[(fpv * 0)] = current_offset_x;                       array[(fpv * 0) + 1] = current_offset_y + scaled_size_y;
        array[(fpv * 1)] = current_offset_x;                       array[(fpv * 1) + 1] = current_offset_y + 0.0f;
        array[(fpv * 2)] = current_offset_x + scaled_size_x;       array[(fpv * 2) + 1] = current_offset_y + 0.0f;
        array[(fpv * 3)] = current_offset_x;                       array[(fpv * 3) + 1] = current_offset_y + scaled_size_y;
        array[(fpv * 4)] = current_offset_x + scaled_size_x;       array[(fpv * 4) + 1] = current_offset_y + 0.0f;
        array[(fpv * 5)] = current_offset_x + scaled_size_x;       array[(fpv * 5) + 1] = current_offset_y + scaled_size_y;

        current_offset_x += scaled_size_x;
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





LV::Variable_Base* TF_Graphics_Component_Reconstructor_Stub__Coordinates::M_construct_product() const
{
    return new TF_Graphics_Component_Reconstructor__Coordinates;
}

void TF_Graphics_Component_Reconstructor_Stub__Coordinates::M_init_constructed_product(LV::Variable_Base *_product) const
{
    LR::Graphics_Component_Reconstructor_Stub::M_init_constructed_product(_product);

    TF_Graphics_Component_Reconstructor__Coordinates* product = (TF_Graphics_Component_Reconstructor__Coordinates*)_product;

}
