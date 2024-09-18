#include <Misc_Draw_Modules/Text_Field/Draw_Module__Text_Field.h>

using namespace LMD;


bool Text_Field_Settings::operator==(const Text_Field_Settings &_other) const
{
    return  font == _other.font &&
            picture == _other.picture &&
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





void Draw_Module__Text_Field::M_update_internal(float _dt)
{
    if(!visible())
        return;

    if(m_configurable_settings.text.size() == 0)
        return;

    bool needs_reconfiguration = m_current_settings != m_configurable_settings;

    if(needs_reconfiguration)
        m_should_recalculate_vertices_before_draw = true;
    else
        m_should_recalculate_vertices_before_draw = false;

    Draw_Module::M_update_internal(_dt);

    if(needs_reconfiguration)
        m_current_settings = m_configurable_settings;
}





LV::Variable_Base* Draw_Module_Stub__Text_Field::M_construct_product() const
{
    return new Draw_Module__Text_Field();
}

void Draw_Module_Stub__Text_Field::M_init_constructed_product(LV::Variable_Base *_product) const
{
    L_ASSERT(resources_manager);

    Draw_Module_Stub::M_init_constructed_product(_product);

    Draw_Module__Text_Field* product = (Draw_Module__Text_Field*)_product;

    Text_Field_Settings& tfs = product->settings();
    tfs.font = resources_manager->get_resource<LR::Font>(font_name);
    tfs.picture = resources_manager->get_resource<LR::Picture>(picture_name);
    tfs.text = text;
    tfs.raw_size_multiplier = raw_size_multiplier;
    tfs.raw_size = raw_size;
    tfs.raw_offset = raw_offset;

    L_ASSERT(horizontal_alignment <= 2);
    L_ASSERT(vertical_alignment <= 2);

    tfs.horizontal_alignment = (Text_Field_Settings::Horizontal_Alignment)horizontal_alignment;
    tfs.vertical_alignment = (Text_Field_Settings::Vertical_Alignment)vertical_alignment;
}
