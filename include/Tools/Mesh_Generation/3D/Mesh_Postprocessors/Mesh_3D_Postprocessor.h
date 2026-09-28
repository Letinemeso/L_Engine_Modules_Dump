#pragma once

#include <Variable_Base.h>
#include <Builder_Stub.h>

#include <Tools/Mesh_Generation/3D/Utility/Mesh_3D_Utilty.h>


namespace LMD
{

    class Mesh_3D_Postprocessor : public LV::Variable_Base
    {
    public:
        INIT_VARIABLE(LMD::Mesh_3D_Postprocessor, LV::Variable_Base)

    public:
        virtual void apply(Mesh_3D_Utility::Points_Vec& _points,
                           const Mesh_3D_Utility::Voxel_Triangles_Map& _mesh_structure) const = 0;

    };


    class Mesh_3D_Postprocessor_Stub : public LV::Builder_Stub
    {
    public:
        INIT_VARIABLE(LMD::Mesh_3D_Postprocessor_Stub, LV::Builder_Stub)

    public:
        INIT_NULL_BUILDER_STUB(Mesh_3D_Postprocessor)

    };

}
