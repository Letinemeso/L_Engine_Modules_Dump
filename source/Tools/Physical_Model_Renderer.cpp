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
    L_ASSERT(m_coordinates_graphic_component->buffer().floats_per_vertex() == 3);
    L_ASSERT(m_coordinates_graphic_component->buffer().size() == 9);
    L_ASSERT(m_draw_module->draw_mode() == GL_TRIANGLES);
}



void Physical_Model_Renderer::draw(const float* _data, unsigned int _size, const LEti::Transformation_Data& _transformation_data) const
{
    L_ASSERT(m_draw_module);
    L_ASSERT(m_coordinates_graphic_component);
    L_ASSERT(_data);

    LR::Buffer& buffer = m_coordinates_graphic_component->buffer();

    m_draw_module->bind_vertex_array();

    LEti::Transformation_Data transformation_data_plug = _transformation_data;

    m_draw_module->set_transformation_data(&transformation_data_plug);
    m_draw_module->set_transformation_data_prev_state(&transformation_data_plug);

    for(unsigned int i = 0; i < _size; i += 9)
    {
        buffer.copy_array(&(_data[i]), 9);

        m_draw_module->update(0.0f);
    }

    m_draw_module->set_transformation_data(nullptr);
    m_draw_module->set_transformation_data_prev_state(nullptr);
}
