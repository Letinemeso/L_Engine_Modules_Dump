#pragma once

#include <Variable_Base.h>
#include <Builder_Stub.h>

#include <Misc_Draw_Modules/Particle_System/Particle_Data.h>


namespace Particle_System
{

    class Particle_Data_Reseter : public LV::Variable_Base
    {
    public:
        DECLARE_VARIABLE;

    private:
        const LEti::Transformation_Data* m_parent_transformation_data = nullptr;

    public:
        inline void set_parent_transformation_data(const LEti::Transformation_Data* _ptr) { m_parent_transformation_data = _ptr; }
        inline const LEti::Transformation_Data* parent_transformation_data() const { return m_parent_transformation_data; }

    public:
        virtual void reset_particle_data(Particle_Data* _particle_data) const = 0;
        virtual Particle_Data* create_particle_data_instance() const = 0;

    };


    class Particle_Data_Reseter_Stub : public LV::Builder_Stub
    {
    public:
        DECLARE_VARIABLE;

    };

}
