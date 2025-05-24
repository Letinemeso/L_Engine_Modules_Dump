#pragma once

#include <Components/Graphics_Component_Reconstructor.h>
#include <Components/Graphics_Component__Default.h>

#include <Misc_Draw_Modules/Particle_Draw_Module/Draw_Module__Particle.h>


namespace LMD
{

    class Graphics_Component_Reconstructor__Buffer_Resizer : public LR::Graphics_Component_Reconstructor
    {
    public:
        INIT_VARIABLE(LMD::Graphics_Component_Reconstructor__Buffer_Resizer, LR::Graphics_Component_Reconstructor)

    public:
        using Data_Vector = LDS::Vector<float>;

    private:
        Data_Vector m_default_data;

    public:
        inline void set_default_data(const Data_Vector& _data) { m_default_data = _data; }

    private:
        Draw_Module__Particle* draw_module() const { L_ASSERT(LV::cast_variable<Draw_Module__Particle>(m_draw_module)); return (Draw_Module__Particle*)m_draw_module; }
        LR::Graphics_Component__Default* graphics_component() const { L_ASSERT(LV::cast_variable<LR::Graphics_Component__Default>(m_graphics_component)); return (LR::Graphics_Component__Default*)m_graphics_component; }

    public:
        void update(float _dt) override;

    };


    class Graphics_Component_Reconstructor_Stub__Buffer_Resizer : public LR::Graphics_Component_Reconstructor_Stub
    {
    public:
        INIT_VARIABLE(LMD::Graphics_Component_Reconstructor_Stub__Buffer_Resizer, LR::Graphics_Component_Reconstructor_Stub)

        INIT_FIELDS
        ADD_FIELD(LDS::Vector<float>, default_data)
        FIELDS_END

    public:
        LDS::Vector<float> default_data;

    public:
        INIT_BUILDER_STUB(Graphics_Component_Reconstructor__Buffer_Resizer)

    };

}
