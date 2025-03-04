#pragma once

#include <Components/Graphics_Component_Reconstructor.h>


namespace LMD
{

    class Graphics_Component_Reconstructor__Particle : public LR::Graphics_Component_Reconstructor
    {
    public:
        INIT_VARIABLE(LMD::Graphics_Component_Reconstructor__Particle, LR::Graphics_Component_Reconstructor)

    private:
        unsigned int m_max_particles_amount = 0;

        LDS::Vector<float> m_default_data;

    public:
        inline void set_default_data(const LDS::Vector& _data) { m_default_data = _data; }

    public:
        void set_max_particles_amount(unsigned int _value);

    public:
        virtual void create_particle(unsigned int _particle_index, float _lifetime) { }
        virtual void destroy_particle(unsigned int _particle_index) { }

    public:
        void update(float _dt) override { }

    };


    class Graphics_Component_Reconstructor_Stub__Particle : public LR::Graphics_Component_Reconstructor_Stub
    {
    public:
        INIT_VARIABLE(LMD::Graphics_Component_Reconstructor_Stub__Particle, LR::Graphics_Component_Reconstructor_Stub)

        INIT_FIELDS
        ADD_FIELD(LDS::Vector<float>, default_data)
        FIELDS_END

    public:
        LDS::Vector<float> default_data;

    public:
        INIT_BUILDER_STUB(Graphics_Component_Reconstructor__Particle)

    };

}
