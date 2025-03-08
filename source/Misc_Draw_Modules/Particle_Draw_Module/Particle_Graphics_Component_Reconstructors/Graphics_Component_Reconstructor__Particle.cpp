#include <Misc_Draw_Modules/Particle_Draw_Module/Particle_Graphics_Component_Reconstructors/Graphics_Component_Reconstructor__Particle.h>

#include <Math_Stuff.h>

#include <Components/Graphics_Component.h>
#include <Draw_Modules/Draw_Module.h>

using namespace LMD;


void Graphics_Component_Reconstructor__Particle::set_max_particles_amount(unsigned int _value)
{
    m_max_particles_amount = _value;

    unsigned int scaled_size = m_default_data.size() * m_max_particles_amount;
    m_graphics_component->buffer().resize(scaled_size);
}



void Graphics_Component_Reconstructor__Particle::M_apply_random_increments(unsigned int _offset)
{
    if(m_random_increments_min.size() == 0)
        return;

    L_ASSERT(m_random_increments_min.size() == m_random_increments_max.size());

    unsigned int increments_amount = m_random_increments_min.size();
    L_ASSERT(m_default_data.size() % increments_amount == 0);

    Data_Vector modifiers(increments_amount, 0.0f);

    for(unsigned int i=0; i<increments_amount; ++i)
        modifiers[i] = LEti::Math::random_number_float(m_random_increments_min[i], m_random_increments_max[i]);

    LR::Buffer::Element_Modification_Func modification_func = [&modifiers, this](float& _element, unsigned int _index)
    {
        float* elements_array = &_element;
        for(unsigned int i=0; i<modifiers.size(); ++i)
            elements_array[i] += modifiers[i];
    };

    m_graphics_component->buffer().modify_buffer(modification_func, _offset, default_data().size(), modifiers.size());
}

void Graphics_Component_Reconstructor__Particle::M_apply_random_multipliers(unsigned int _offset)
{
    if(m_random_multipliers_min.size() == 0)
        return;

    L_ASSERT(m_random_multipliers_min.size() == m_random_multipliers_max.size());

    unsigned int multipliers_amount = m_random_multipliers_min.size();
    L_ASSERT(m_default_data.size() % multipliers_amount == 0);

    Data_Vector modifiers(multipliers_amount, 0.0f);

    for(unsigned int i=0; i<multipliers_amount; ++i)
        modifiers[i] = LEti::Math::random_number_float(m_random_multipliers_min[i], m_random_multipliers_max[i]);

    LR::Buffer::Element_Modification_Func modification_func = [&modifiers, this](float& _element, unsigned int _index)
    {
        float* elements_array = &_element;
        for(unsigned int i=0; i<modifiers.size(); ++i)
            elements_array[i] *= modifiers[i];
    };

    m_graphics_component->buffer().modify_buffer(modification_func, _offset, default_data().size(), modifiers.size());
}



void Graphics_Component_Reconstructor__Particle::create_particle(unsigned int _particle_index)
{
    unsigned int offset = _particle_index * m_default_data.size();
    m_draw_module->bind_vertex_array();
    m_graphics_component->buffer().copy_array(m_default_data.raw_data(), m_default_data.size(), offset);

    M_apply_random_increments(offset);
    M_apply_random_multipliers(offset);
}

void Graphics_Component_Reconstructor__Particle::destroy_particle(unsigned int _particle_index)
{

}



void Graphics_Component_Reconstructor__Particle::update(float _dt)
{

}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Graphics_Component_Reconstructor_Stub__Particle)

BUILDER_STUB_INITIALIZATION_FUNC(Graphics_Component_Reconstructor_Stub__Particle)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_default_data(default_data);

    if(random_increments.size() > 0)
    {
        L_ASSERT(random_increments.size() % 2 == 0);

        unsigned int minmax_values_amount = random_increments.size() / 2;
        L_ASSERT(default_data.size() % minmax_values_amount == 0);

        LDS::Vector<float> min(minmax_values_amount);
        LDS::Vector<float> max(minmax_values_amount);
        for(unsigned int i=0; i<random_increments.size(); i += 2)
        {
            min.push(random_increments[i]);
            max.push(random_increments[i + 1]);
        }

        product->set_random_increments((LDS::Vector<float>&&)min, (LDS::Vector<float>&&)max);
    }

    if(random_multipliers.size() > 0)
    {
        L_ASSERT(random_multipliers.size() % 2 == 0);

        unsigned int minmax_values_amount = random_multipliers.size() / 2;
        L_ASSERT(default_data.size() % minmax_values_amount == 0);

        LDS::Vector<float> min(minmax_values_amount);
        LDS::Vector<float> max(minmax_values_amount);
        for(unsigned int i=0; i<random_multipliers.size(); i += 2)
        {
            min.push(random_multipliers[i]);
            max.push(random_multipliers[i + 1]);
        }

        product->set_random_multipliers((LDS::Vector<float>&&)min, (LDS::Vector<float>&&)max);
    }
}
