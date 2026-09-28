#pragma once

#include <Tools/Mesh_Generation/3D/Mesh_Postprocessors/Mesh_3D_Postprocessor.h>


namespace LMD
{

    class Mesh_3D_Postprocessor__Random_Stride : public Mesh_3D_Postprocessor
    {
    public:
        INIT_VARIABLE(LMD::Mesh_3D_Postprocessor__Random_Stride, LMD::Mesh_3D_Postprocessor)

    private:
        float m_min_stride = 0.0f;
        float m_max_stride = 0.0f;

    public:
        inline void set_min_stride(float _value) { m_min_stride = _value; }
        inline void set_max_stride(float _value) { m_max_stride = _value; }

    public:
        void apply(Mesh_3D_Utility::Points_Vec& _points,
                   const Mesh_3D_Utility::Voxel_Triangles_Map& _mesh_structure) const override;

    };


    class Mesh_3D_Postprocessor_Stub__Random_Stride : public Mesh_3D_Postprocessor_Stub
    {
    public:
        INIT_VARIABLE(LMD::Mesh_3D_Postprocessor_Stub__Random_Stride, LMD::Mesh_3D_Postprocessor_Stub)

        INIT_FIELDS
        ADD_FIELD(float, min_stride)
        ADD_FIELD(float, max_stride)
        FIELDS_END

    public:
        float min_stride = 0.0f;
        float max_stride = 0.0f;

    public:
        INIT_DEFAULT_BUILDER_STUB(Mesh_3D_Postprocessor__Random_Stride)

        INIT_BUILDER_STUB_SETTERS
        ADD_BUILDER_STUB_SETTER(set_min_stride, min_stride)
        ADD_BUILDER_STUB_SETTER(set_max_stride, max_stride)
        BUILDER_STUB_SETTERS_END

    };

}
