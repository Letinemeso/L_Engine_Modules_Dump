#pragma once

#include <vec2.hpp>
#include <vec3.hpp>

#include <Components/Graphics_Component_Reconstructor.h>
#include <Components/Graphics_Component__Default.h>
#include <Resources/Font.h>

#include <Misc_Draw_Modules/Text_Field/Graphics_Component_Reconstructors/String_Tools.h>


namespace LMD
{

    class Text_Field_Settings;


    class TF_Graphics_Component_Reconstructor__Coordinates : public LR::Graphics_Component_Reconstructor
    {
    public:
        INIT_VARIABLE(LMD::TF_Graphics_Component_Reconstructor__Coordinates, LR::Graphics_Component_Reconstructor)

    private:
        float m_highest_letter = 0.0f;

    private:
        LR::Graphics_Component__Default* graphics_component() const { L_ASSERT(LV::cast_variable<LR::Graphics_Component__Default>(m_graphics_component)); return (LR::Graphics_Component__Default*)m_graphics_component; }

    private:
        float M_calculate_next_line_offset_x(const std::string& _text, unsigned int _offset, float _raw_scale, const Text_Field_Settings& _settings) const;
        float M_get_highest_letter_size(const Text_Field_Settings& _settings) const;
        glm::vec2 M_calculate_raw_size(const Text_Field_Settings& _settings) const;
        float M_calculate_raw_scale(const Text_Field_Settings& _settings, const glm::vec2& _raw_size) const;

    public:
        void update(float _dt) override;

    };


    class TF_Graphics_Component_Reconstructor_Stub__Coordinates : public LR::Graphics_Component_Reconstructor_Stub
    {
    public:
        INIT_VARIABLE(LMD::TF_Graphics_Component_Reconstructor_Stub__Coordinates, LR::Graphics_Component_Reconstructor_Stub)

    protected:
        INIT_BUILDER_STUB(TF_Graphics_Component_Reconstructor__Coordinates)

    };

}
