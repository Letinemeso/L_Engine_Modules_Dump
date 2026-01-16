#include <Tools/Physical_Model_Renderer.h>

#include <glew.h>

using namespace LMD;


Physical_Model_Renderer::Physical_Model_Renderer()
{

}

Physical_Model_Renderer::~Physical_Model_Renderer()
{
    delete m_draw_module;
}



void Physical_Model_Renderer::set_draw_module(LR::Draw_Module* _ptr, unsigned int _coordinates_graphics_component_index)
{
    delete m_draw_module;
    m_draw_module = _ptr;

    m_coordinates_graphic_component = (LR::Graphics_Component__Default*)m_draw_module->get_graphics_component_with_buffer_index(_coordinates_graphics_component_index);
    L_ASSERT(LV::cast_variable<LR::Graphics_Component__Default>(m_coordinates_graphic_component));
    L_ASSERT(m_coordinates_graphic_component);
    L_ASSERT(m_draw_module->draw_mode() == GL_TRIANGLES);
}



void Physical_Model_Renderer::draw(const float* _data, unsigned int _size, const LEti::Transformation_Data& _transformation_data) const
{
    L_ASSERT(m_draw_module);
    L_ASSERT(m_coordinates_graphic_component);
    L_ASSERT(_data);

    LR::Buffer& buffer = m_coordinates_graphic_component->buffer();

    m_draw_module->apply_draw_rule();

    LEti::Transformation_Data transformation_data_plug = _transformation_data;

    m_draw_module->set_transformation_data(&transformation_data_plug);
    m_draw_module->set_transformation_data_prev_state(&transformation_data_plug);

    LDS::Vector<float> raw_data(buffer.floats_per_vertex() * 3);

    for(unsigned int offset_i = 0; offset_i < _size; offset_i += 9)
    {
        const float* data_with_offset = _data + offset_i;

        raw_data.mark_empty();
        for(unsigned int v_i = 0; v_i < 3; ++v_i)
        {
            for(unsigned int i = 0; i < buffer.floats_per_vertex(); ++i)
                raw_data.push(data_with_offset[v_i * 3 + i]);
        }

        buffer.copy_array(raw_data.raw_data(), raw_data.size());

        m_draw_module->update(0.0f);
    }

    m_draw_module->set_transformation_data(nullptr);
    m_draw_module->set_transformation_data_prev_state(nullptr);
}
