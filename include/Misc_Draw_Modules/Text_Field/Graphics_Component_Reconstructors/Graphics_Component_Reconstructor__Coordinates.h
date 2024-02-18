#pragma once

#include <vec2.hpp>
#include <vec3.hpp>

#include <Components/Graphics_Component_Reconstructor.h>


namespace LMD
{

    class Text_Field_Settings;


    class TF_Graphics_Component_Reconstructor__Coordinates : public LR::Graphics_Component_Reconstructor
    {
    public:
        INIT_VARIABLE(LMD::TF_Graphics_Component_Reconstructor__Coordinates, LR::Graphics_Component_Reconstructor)

    private:
        glm::vec2 M_calculate_raw_size(const Text_Field_Settings& _settings) const;
        float M_calculate_raw_scale(const Text_Field_Settings& _settings, const glm::vec2& _raw_size) const;

    public:
        void reconstruct_graphics_component() override;

    };


    class TF_Graphics_Component_Reconstructor_Stub__Coordinates : public LR::Graphics_Component_Reconstructor_Stub
    {
    public:
        INIT_VARIABLE(LMD::TF_Graphics_Component_Reconstructor_Stub__Coordinates, LR::Graphics_Component_Reconstructor_Stub)

    protected:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* _product) const override;

    };

}
