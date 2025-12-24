#pragma once

#include <Shader/Shader_Components/Shader_Component.h>


namespace LMD
{

    class Shader_Component__Particle_Settings : public LR::Shader_Component
    {
    public:
        INIT_VARIABLE(LMD::Shader_Component__Particle_Settings, LR::Shader_Component)

    private:
        int m_max_particles_uniform = -1;
        std::string m_max_particles_uniform_name;

    public:
        inline void set_max_particles_uniform_name(const std::string& _value) { m_max_particles_uniform_name = _value; }

    public:
        void init(unsigned int _opengl_program_handle) override;

    public:
        void update(const LR::Draw_Module* _draw_module) override;

    };


    class Shader_Component_Stub__Particle_Settings : public LR::Predefined_Shader_Component_Stub
    {
    public:
        INIT_VARIABLE(LMD::Shader_Component_Stub__Particle_Settings, LR::Predefined_Shader_Component_Stub)

        INIT_FIELDS
        ADD_FIELD(unsigned int, invocations_amount)
        ADD_FIELD(std::string, max_particles_uniform_name)
        FIELDS_END

    public:
        unsigned int invocations_amount = 128;
        std::string max_particles_uniform_name = "max_particles";

    public:
        INIT_BUILDER_STUB(Shader_Component__Particle_Settings)

    };

}
