#pragma once

#include <Variable_Base.h>
#include <Builder_Stub.h>

#include <Tools/Mesh_Generation/3D/Utility/Mesh_3D_Utilty.h>


namespace LMD
{

    class Mesh_Data_Extractor : public LV::Variable_Base
    {
    public:
        INIT_VARIABLE(LMD::Mesh_Data_Extractor, LV::Variable_Base)

    public:
        virtual LDS::Vector<float> extract(const Mesh_3D_Utility::Triangles_Vec& _triangles,
                                           const Mesh_3D_Utility::Points_Vec& _points_cache) const = 0;

    };


    class Mesh_Data_Extractor_Stub : public LV::Builder_Stub
    {
    public:
        INIT_VARIABLE(LMD::Mesh_Data_Extractor_Stub, LV::Builder_Stub)

    public:
        INIT_NULL_BUILDER_STUB(Mesh_Data_Extractor)

    };

}
