#pragma once

#include <vec2.hpp>
#include <vec3.hpp>

#include <Components/Graphics_Component_Reconstructor.h>
#include <Components/Graphics_Component__Default.h>

#include <Misc_Draw_Modules/Text_Field/Graphics_Component_Reconstructors/String_Tools.h>


namespace LMD
{

    class Text_Field_Settings;


    class TF_Graphics_Component_Reconstructor__Colors : public LR::Graphics_Component_Reconstructor
    {
    public:
        INIT_VARIABLE(LMD::TF_Graphics_Component_Reconstructor__Colors, LR::Graphics_Component_Reconstructor)

    private:
        LR::Graphics_Component__Default* graphics_component() const { L_ASSERT(LV::cast_variable<LR::Graphics_Component__Default>(m_graphics_component)); return (LR::Graphics_Component__Default*)m_graphics_component; }

    public:
        void update(float _dt) override;

    };


    class TF_Graphics_Component_Reconstructor_Stub__Colors : public LR::Graphics_Component_Reconstructor_Stub
    {
    public:
        INIT_VARIABLE(LMD::TF_Graphics_Component_Reconstructor_Stub__Colors, LR::Graphics_Component_Reconstructor_Stub)

    protected:
        INIT_BUILDER_STUB(TF_Graphics_Component_Reconstructor__Colors)

    };

}
