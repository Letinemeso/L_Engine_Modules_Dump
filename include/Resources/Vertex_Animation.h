#pragma once

#include <Variable_Base.h>
#include <Builder_Stub.h>

#include <Resources/Model.h>


namespace LMD
{

    class Vertex_Animation : public Model
    {
    public:
        INIT_VARIABLE(LMD::Vertex_Animation, LMD::Model)

    private:
        unsigned int m_frames_amount = 0;
        unsigned int m_vertices_per_frame = 0;

    public:
        inline void set_frames_amount(unsigned int _value) { m_frames_amount = _value; }
        inline void set_vertices_per_frame(unsigned int _value) { m_vertices_per_frame = _value; }

    public:
        inline unsigned int frames_amount() const { return m_frames_amount; }
        inline unsigned int vertices_per_frame() const { return m_vertices_per_frame; }

    };


    class Vertex_Animation_Stub : public LV::Builder_Stub
    {
    public:
        INIT_VARIABLE(LMD::Vertex_Animation_Stub, LV::Builder_Stub)

        INIT_CHILDS_LISTS
        ADD_CHILDS_LIST("Frame_*", &frames)
        CHILDS_LISTS_END

        OVERRIDE_ON_VALUES_ASSIGNED

    public:
        LV::Variable_Base::Childs_List frames;

    public:
        unsigned int frames_amount = 0;
        unsigned int vertices_per_frame = 0;

        LDS::Vector<float> geometry_data;
        LDS::Vector<float> texture_data;
        LDS::Vector<float> normals_data;

    public:
        INIT_BUILDER_STUB(Vertex_Animation)

    public:
        ~Vertex_Animation_Stub();

    };

}
