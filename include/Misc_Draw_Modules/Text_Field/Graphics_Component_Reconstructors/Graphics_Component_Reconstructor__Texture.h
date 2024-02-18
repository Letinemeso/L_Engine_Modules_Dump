#pragma once

#include <vec2.hpp>
#include <vec3.hpp>

#include <Components/Graphics_Component_Reconstructor.h>


namespace LMD
{

    class Text_Field_Settings;


    class Graphics_Component_Reconstructor__Texture : public LR::Graphics_Component_Reconstructor
    {
    public:
        INIT_VARIABLE(LMD::Graphics_Component_Reconstructor__Texture, LR::Graphics_Component_Reconstructor)

    public:
        void reconstruct_graphics_component() override;

    };


    class Graphics_Component_Reconstructor_Stub__Texture : public LR::Graphics_Component_Reconstructor_Stub
    {
    public:
        INIT_VARIABLE(LMD::Graphics_Component_Reconstructor_Stub__Texture, LR::Graphics_Component_Reconstructor_Stub)

    protected:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* _product) const override;

    };

}
