#include <Misc_Draw_Modules/Particle_Draw_Module/Graphics_Components/Graphics_Component__Particles_Amount_Reader.h>

#include <glew.h>

#include <Object.h>

#include <Draw_Modules/Draw_Module.h>

using namespace LMD;


Graphics_Component__Particles_Amount_Reader::Graphics_Component__Particles_Amount_Reader()
{
    glGenBuffers(1, &m_counter_buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_counter_buffer);
    unsigned int initial_data[2] = { 0, 0 };
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint) * 2, initial_data, GL_DYNAMIC_COPY);

    glGenBuffers(1, &m_read_buffer);
    glBindBuffer(GL_COPY_READ_BUFFER, m_read_buffer);
    glBufferData(GL_COPY_READ_BUFFER, sizeof(GLuint) * 2, nullptr, GL_STREAM_READ);
}

Graphics_Component__Particles_Amount_Reader::~Graphics_Component__Particles_Amount_Reader()
{
    glDeleteBuffers(1, &m_counter_buffer);
    glDeleteBuffers(1, &m_read_buffer);
}



void Graphics_Component__Particles_Amount_Reader::M_extract_data() const
{
    glBindBuffer(GL_COPY_READ_BUFFER, m_read_buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_counter_buffer);
    glCopyBufferSubData(GL_SHADER_STORAGE_BUFFER, GL_COPY_READ_BUFFER, 0, 0, sizeof(GLuint) * 2);

    unsigned int alive_particles = 0;
    glGetBufferSubData(GL_COPY_READ_BUFFER, 0, sizeof(GLuint), &alive_particles);

    draw_module()->update_alive_particles_amount(alive_particles);
}

void Graphics_Component__Particles_Amount_Reader::M_fill_requested_particles() const
{
    unsigned int requested_amount = draw_module()->requested_particles_amount();
    if(requested_amount == 0)
        return;

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_counter_buffer);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(GLuint), sizeof(GLuint), &requested_amount);
}



unsigned int Graphics_Component__Particles_Amount_Reader::layout_index() const
{
    return 0xFFFFFFFF;
}

unsigned int Graphics_Component__Particles_Amount_Reader::vertices_amount() const
{
    return 0;
}

unsigned int Graphics_Component__Particles_Amount_Reader::required_compute_shader_invocations() const
{
    return 0;
}

void Graphics_Component__Particles_Amount_Reader::setup_buffer() const
{

}

void Graphics_Component__Particles_Amount_Reader::update(float _dt)
{
    if(m_update_frequency > 0.0001f)
    {
        m_timer.update(_dt);
        if(m_timer.is_active())
            return;

        m_timer.start(m_update_frequency);
    }

    M_extract_data();
}

void Graphics_Component__Particles_Amount_Reader::prepare_to_draw() const
{

}

void Graphics_Component__Particles_Amount_Reader::bind_for_computation() const
{
    L_ASSERT(m_binding_point_index != 0xFFFFFFFF);

    M_fill_requested_particles();

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_binding_point_index, m_counter_buffer);
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Graphics_Component_Stub__Particles_Amount_Reader)

BUILDER_STUB_INITIALIZATION_FUNC(Graphics_Component_Stub__Particles_Amount_Reader)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    product->set_binding_point_index(binding_point_index);
    product->set_update_frequency(update_frequency);
}
