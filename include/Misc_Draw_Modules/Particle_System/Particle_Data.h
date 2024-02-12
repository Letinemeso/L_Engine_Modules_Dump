#pragma once

#include <Draw_Modules/Draw_Module_Base.h>


namespace Particle_System
{

    class Particle_Data
    {
    public:
        virtual ~Particle_Data() { }

    public:
        virtual bool is_alive() const = 0;

    public:
        virtual void update(float _dt, LR::Draw_Module* _draw_module) = 0;

    };

}
