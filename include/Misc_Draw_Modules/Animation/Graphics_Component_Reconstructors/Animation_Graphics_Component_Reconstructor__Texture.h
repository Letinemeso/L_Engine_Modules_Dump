#pragma once

#include <vec2.hpp>
#include <vec3.hpp>

#include <Components/Graphics_Component_Reconstructor.h>


namespace LMD
{

    class Animation_Graphics_Component_Reconstructor__Texture : public LR::Graphics_Component_Reconstructor
    {
    public:
        INIT_VARIABLE(LMD::Animation_Graphics_Component_Reconstructor__Texture, LR::Graphics_Component_Reconstructor)

    private:
        unsigned int m_current_frame = 0;

    public:
        void update(float _dt) override;

    };


    class Animation_Graphics_Component_Reconstructor_Stub__Texture : public LR::Graphics_Component_Reconstructor_Stub
    {
    public:
        INIT_VARIABLE(LMD::Animation_Graphics_Component_Reconstructor_Stub__Texture, LR::Graphics_Component_Reconstructor_Stub)

    protected:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* _product) const override;

    };

}
