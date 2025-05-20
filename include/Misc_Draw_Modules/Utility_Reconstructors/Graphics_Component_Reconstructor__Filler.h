#pragma once

#include <Data_Structures/Vector.h>

#include <Components/Graphics_Component_Reconstructor.h>


namespace LMD
{

    class Graphics_Component_Reconstructor__Filler : public LR::Graphics_Component_Reconstructor
    {
    public:
        INIT_VARIABLE(LMD::Graphics_Component_Reconstructor__Filler, LR::Graphics_Component_Reconstructor)

    private:
        unsigned int m_copies_amount = 0;
        LDS::Vector<float> m_data_per_copy;

    public:
        inline void set_copies_amount(unsigned int _value) { m_copies_amount = _value; }
        inline void set_data_per_copy(const LDS::Vector<float>& _value) { m_data_per_copy = _value; }

    protected:
        void M_on_graphics_component_injected() override;

    public:
        void update(float _dt) override { };

    };


    class Graphics_Component_Reconstructor_Stub__Filler : public LR::Graphics_Component_Reconstructor_Stub
    {
    public:
        INIT_VARIABLE(LMD::Graphics_Component_Reconstructor_Stub__Filler, LR::Graphics_Component_Reconstructor_Stub)

        INIT_FIELDS
        ADD_FIELD(unsigned int, copies_amount)
        ADD_FIELD(LDS::Vector<float>, data_per_copy)
        FIELDS_END

    public:
        unsigned int copies_amount = 0;
        LDS::Vector<float> data_per_copy;

    public:
        INIT_BUILDER_STUB(Graphics_Component_Reconstructor__Filler)

    };

}
