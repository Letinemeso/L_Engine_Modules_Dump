#pragma once

#include <Components/Graphics_Component_Reconstructor.h>
#include <Components/Graphics_Component__Default.h>


namespace LMD
{

    class Graphics_Component_Reconstructor__Particle : public LR::Graphics_Component_Reconstructor
    {
    public:
        INIT_VARIABLE(LMD::Graphics_Component_Reconstructor__Particle, LR::Graphics_Component_Reconstructor)

    public:
        using Data_Vector = LDS::Vector<float>;

    private:
        unsigned int m_max_particles_amount = 0;

        Data_Vector m_default_data;
        Data_Vector m_random_increments_min;
        Data_Vector m_random_increments_max;
        Data_Vector m_random_multipliers_min;
        Data_Vector m_random_multipliers_max;

    public:
        inline void set_default_data(const Data_Vector& _data) { m_default_data = _data; }
        inline void set_random_increments(const Data_Vector& _min, const Data_Vector& _max) { m_random_increments_min = _min; m_random_increments_max = _max; }
        inline void set_random_multipliers(const Data_Vector& _min, const Data_Vector& _max) { m_random_multipliers_min = _min; m_random_multipliers_max = _max; }
        inline void set_random_increments(Data_Vector&& _min, Data_Vector&& _max) { m_random_increments_min = (Data_Vector&&)_min; m_random_increments_max = (Data_Vector&&)_max; }
        inline void set_random_multipliers(Data_Vector&& _min, Data_Vector&& _max) { m_random_multipliers_min = (Data_Vector&&)_min; m_random_multipliers_max = (Data_Vector&&)_max; }

        inline const Data_Vector& default_data() const { return m_default_data; }
        inline unsigned int max_particles_amount() const { return m_max_particles_amount; }

    private:
        LR::Graphics_Component__Default* graphics_component() const { L_ASSERT(LV::cast_variable<LR::Graphics_Component__Default>(m_graphics_component)); return (LR::Graphics_Component__Default*)m_graphics_component; }

    public:
        void set_max_particles_amount(unsigned int _value);

    protected:
        void M_apply_random_increments(unsigned int _offset);
        void M_apply_random_multipliers(unsigned int _offset);

    public:
        virtual void create_particle(unsigned int _particle_index);
        virtual void destroy_particle(unsigned int _particle_index);

    public:
        void update(float _dt) override;

    };


    class Graphics_Component_Reconstructor_Stub__Particle : public LR::Graphics_Component_Reconstructor_Stub
    {
    public:
        INIT_VARIABLE(LMD::Graphics_Component_Reconstructor_Stub__Particle, LR::Graphics_Component_Reconstructor_Stub)

        INIT_FIELDS
        ADD_FIELD(LDS::Vector<float>, default_data)
        ADD_FIELD(LDS::Vector<float>, random_increments)
        ADD_FIELD(LDS::Vector<float>, random_multipliers)
        FIELDS_END

    public:
        LDS::Vector<float> default_data;
        LDS::Vector<float> random_increments;      //  for default_data = { 1.0f, 2.0f, 3.0f } should looks like { { 0.0f, 2.0f }, { 1.0f, 3.0f }, { 2.0f, 4.0f } }
        LDS::Vector<float> random_multipliers;     //  (size / 2 = default_data.floats_per_vertex)

    public:
        INIT_BUILDER_STUB(Graphics_Component_Reconstructor__Particle)

    };

}
