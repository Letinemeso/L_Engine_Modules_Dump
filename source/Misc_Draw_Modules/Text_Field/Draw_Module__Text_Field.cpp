#include <Misc_Draw_Modules/Text_Field/Draw_Module__Text_Field.h>

using namespace LMD;


bool Text_Field_Settings::operator==(const Text_Field_Settings &_other) const
{
    return  font == _other.font &&
            horizontal_alignment == _other.horizontal_alignment &&
            vertical_alignment == _other.vertical_alignment &&
            LEti::Math::vecs_are_equal(raw_offset, _other.raw_offset) &&
            LEti::Math::floats_are_equal(raw_size, _other.raw_size) &&
            text == _other.text;
}

bool Text_Field_Settings::operator!=(const Text_Field_Settings &_other) const
{
    return !(*this == _other);
}





glm::vec2 Draw_Module__Text_Field::M_calculate_raw_size() const
{
    glm::vec2 result;

    if(m_current_settings.text.size() == 0)
        return result;

    result.x = m_current_settings.font->get_letter_data(m_current_settings.text[0]).size_x;
    result.y = m_current_settings.font->get_letter_data(m_current_settings.text[0]).size_y;

    for(unsigned int i=1; i<m_current_settings.text.size(); ++i)
    {
        result.x += m_current_settings.font->get_letter_data(m_current_settings.text[i]).size_x;
        float ch = m_current_settings.font->get_letter_data(m_current_settings.text[i]).size_y;
        if(ch > result.y)
            result.y = ch;
    }

    return result;
}

float Draw_Module__Text_Field::M_calculate_raw_scale(const glm::vec2& _raw_size) const
{
    if(m_current_settings.raw_size < 0.0f)
        return m_current_settings.raw_size_multiplier;

    float result = 1.0f;

    if(_raw_size.x > _raw_size.y)
        result = m_current_settings.raw_size / _raw_size.x;
    else
        result = m_current_settings.raw_size / _raw_size.y;

    return result * m_current_settings.raw_size_multiplier;
}


void Draw_Module__Text_Field::M_construct_coords(float *_coords, unsigned int _amount, unsigned int _amount_per_character)
{
    unsigned int fpv = m_coords->buffer().floats_per_vertex();
    L_ASSERT(fpv > 1);    //  single-dimentional font is not supported :D

    for(unsigned int i=0; i<_amount; ++i)
        _coords[i] = 0.0f;

    glm::vec2 raw_size = M_calculate_raw_size();
    float raw_scale = M_calculate_raw_scale(raw_size);

    float current_width = 0.0f;

    auto construct_character = [this, &current_width, raw_scale, _coords, _amount_per_character, fpv](unsigned int _character_index)
    {
        float* array = &(_coords[_character_index * _amount_per_character]);

        const LR::Letter_Data& letter_data = m_current_settings.font->get_letter_data(m_current_settings.text[_character_index]);

        float scaled_size_x = letter_data.size_x * raw_scale;
        float scaled_size_y = letter_data.size_y * raw_scale;

        array[(fpv * 0) + 0] = current_width;                       array[(fpv * 0) + 1] = scaled_size_y;
        array[(fpv * 1) + 0] = current_width;                       array[(fpv * 1) + 1] = 0.0f;
        array[(fpv * 2) + 0] = current_width + scaled_size_x;       array[(fpv * 2) + 1] = 0.0f;
        array[(fpv * 3) + 0] = current_width;                       array[(fpv * 3) + 1] = scaled_size_y;
        array[(fpv * 4) + 0] = current_width + scaled_size_x;       array[(fpv * 4) + 1] = 0.0f;
        array[(fpv * 5) + 0] = current_width + scaled_size_x;       array[(fpv * 5) + 1] = scaled_size_y;

        current_width += scaled_size_x;
    };

    for(unsigned int i=0; i<m_current_settings.text.size(); ++i)
        construct_character(i);

    glm::vec3 raw_offset = m_current_settings.raw_offset;

    if(m_current_settings.horizontal_alignment == Text_Field_Settings::Horizontal_Alignment::Center)
        raw_offset.x -= raw_size.x / 2.0f;
    else if(m_current_settings.horizontal_alignment == Text_Field_Settings::Horizontal_Alignment::Right)
        raw_offset.x -= raw_size.x;

    if(m_current_settings.vertical_alignment == Text_Field_Settings::Vertical_Alignment::Center)
        raw_offset.y -= raw_size.y / 2.0f;
    else if(m_current_settings.vertical_alignment == Text_Field_Settings::Vertical_Alignment::Top)
        raw_offset.y -= raw_size.y;

    raw_offset *= raw_scale;

    for(unsigned int vertex_i = 0; vertex_i < _amount; vertex_i += fpv)
    {
        for(unsigned int i = 0; i < fpv && i < 3; ++i)
            _coords[vertex_i + i] += raw_offset[i];
    }
}

void Draw_Module__Text_Field::M_construct_colors(float* _colors, unsigned int _amount)
{
    for(unsigned int i=0; i<_amount; ++i)
        _colors[i] = 1.0f;
}

void Draw_Module__Text_Field::M_construct_texture_coords(float* _texture_coords, unsigned int _amount, unsigned int _amount_per_character)
{
    unsigned int fpv = m_texture->buffer().floats_per_vertex();
    L_ASSERT(fpv > 1);    //  single-dimentional font is not supported :D

    for(unsigned int i=0; i<_amount; ++i)
        _texture_coords[i] = 0.0f;

    auto construct_character = [this, _texture_coords, _amount_per_character, fpv](unsigned int _character_index)
    {
        float* array = &(_texture_coords[_character_index * _amount_per_character]);

        const LR::Letter_Data& letter_data = m_current_settings.font->get_letter_data(m_current_settings.text[_character_index]);

        array[(fpv * 0) + 0] = letter_data.pos_x;                           array[(fpv * 0) + 1] = letter_data.pos_y + letter_data.size_y;
        array[(fpv * 1) + 0] = letter_data.pos_x;                           array[(fpv * 1) + 1] = letter_data.pos_y;
        array[(fpv * 2) + 0] = letter_data.pos_x + letter_data.size_x;      array[(fpv * 2) + 1] = letter_data.pos_y;
        array[(fpv * 3) + 0] = letter_data.pos_x;                           array[(fpv * 3) + 1] = letter_data.pos_y + letter_data.size_y;
        array[(fpv * 4) + 0] = letter_data.pos_x + letter_data.size_x;      array[(fpv * 4) + 1] = letter_data.pos_y;
        array[(fpv * 5) + 0] = letter_data.pos_x + letter_data.size_x;      array[(fpv * 5) + 1] = letter_data.pos_y + letter_data.size_y;
    };

    for(unsigned int i=0; i<m_current_settings.text.size(); ++i)
        construct_character(i);
}


void Draw_Module__Text_Field::M_reconfigure()
{
    m_current_settings = m_configurable_settings;

    if(m_configurable_settings.text.size() == 0)
        return;

    m_texture->set_picture(m_current_settings.font->picture());

    const unsigned int coords_per_character = m_coords->buffer().floats_per_vertex() * 6;            //  theese magic numbers should be taken from shader
    const unsigned int colors_per_character = m_colors->buffer().floats_per_vertex() * 6;
    const unsigned int texture_coords_per_character = m_texture->buffer().floats_per_vertex() * 6;

    unsigned int coords_amount = coords_per_character * m_current_settings.text.size();
    unsigned int colors_amount = colors_per_character * m_current_settings.text.size();
    unsigned int texture_coords_amount = texture_coords_per_character * m_current_settings.text.size();

    float* coords_buffer = new float[coords_amount];
    float* colors_buffer = new float[colors_amount];
    float* texture_coords_buffer = new float[texture_coords_amount];

    M_construct_coords(coords_buffer, coords_amount, coords_per_character);
    M_construct_colors(colors_buffer, colors_amount);
    M_construct_texture_coords(texture_coords_buffer, texture_coords_amount, texture_coords_per_character);

    glBindVertexArray(vertex_array());

    m_coords->buffer().use_array(coords_buffer, coords_amount);
    m_colors->buffer().use_array(colors_buffer, colors_amount);
    m_texture->buffer().use_array(texture_coords_buffer, texture_coords_amount);
    m_texture->reconfigure_texture_coords();

    m_vertices_amount = coords_amount / m_coords->buffer().floats_per_vertex();
}



void Draw_Module__Text_Field::update(float _dt)
{
    if(m_current_settings != m_configurable_settings)
        M_reconfigure();

    if(m_current_settings.text.size() == 0)
        return;

    Draw_Module::update(_dt);
}





Draw_Module_Stub__Text_Field::~Draw_Module_Stub__Text_Field()
{
    delete coords_stub;
    delete colors_stub;
    delete texture_stub;
}



LV::Variable_Base* Draw_Module_Stub__Text_Field::M_construct_product() const
{
    return new Draw_Module__Text_Field();
}

void Draw_Module_Stub__Text_Field::M_init_constructed_product(LV::Variable_Base *_product) const
{
    L_ASSERT(coords_stub);
    L_ASSERT(colors_stub);
    L_ASSERT(texture_stub);

    Draw_Module_Stub::M_init_constructed_product(_product);

    Draw_Module__Text_Field* product = (Draw_Module__Text_Field*)_product;

    Text_Field_Settings& tfs = product->settings();
    tfs.font = texture_stub->resources_manager->get_font(font_name);
    tfs.text = text;
    tfs.raw_size_multiplier = raw_size_multiplier;
    tfs.raw_size = raw_size;
    tfs.raw_offset = raw_offset;

    L_ASSERT(horizontal_alignment <= 2);
    L_ASSERT(vertical_alignment <= 2);

    tfs.horizontal_alignment = (Text_Field_Settings::Horizontal_Alignment)horizontal_alignment;
    tfs.vertical_alignment = (Text_Field_Settings::Vertical_Alignment)vertical_alignment;

    LR::Graphics_Component* coords = (LR::Graphics_Component*)coords_stub->construct();
    LR::Graphics_Component* colors = (LR::Graphics_Component*)colors_stub->construct();
    LR::Graphics_Component__Texture* texture = (LR::Graphics_Component__Texture*)texture_stub->construct();

    product->set_coords_component(coords);
    product->set_colors_component(colors);
    product->set_texture_component(texture);

    product->add_graphics_component(coords);
    product->add_graphics_component(colors);
    product->add_graphics_component(texture);
}

























































