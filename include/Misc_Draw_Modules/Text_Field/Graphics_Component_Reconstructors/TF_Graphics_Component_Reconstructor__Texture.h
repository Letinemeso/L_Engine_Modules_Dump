#pragma once

#include <vec2.hpp>
#include <vec3.hpp>

#include <Components/Graphics_Component_Reconstructor.h>
#include <Misc_Draw_Modules/Text_Field/Graphics_Component_Reconstructors/String_Tools.h>


namespace LMD
{

    class Text_Field_Settings;


    class TF_Graphics_Component_Reconstructor__Texture : public LR::Graphics_Component_Reconstructor
    {
    public:
        INIT_VARIABLE(LMD::TF_Graphics_Component_Reconstructor__Texture, LR::Graphics_Component_Reconstructor)

    public:
        void update(float _dt) override;

    };


    class TF_Graphics_Component_Reconstructor_Stub__Texture : public LR::Graphics_Component_Reconstructor_Stub
    {
    public:
        INIT_VARIABLE(LMD::TF_Graphics_Component_Reconstructor_Stub__Texture, LR::Graphics_Component_Reconstructor_Stub)

    protected:
        INIT_BUILDER_STUB(TF_Graphics_Component_Reconstructor__Texture)

    };

}
