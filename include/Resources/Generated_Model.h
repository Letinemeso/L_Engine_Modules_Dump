#pragma once

#include <Resources/Model.h>
#include <Tools/Voxel/Voxel_3D/Voxel_3D_Controller.h>
#include <Tools/Voxel/Voxel_3D/Voxel_3D_Generator.h>
#include <Tools/Mesh_Generation/3D/Mesh_Extractor_3D.h>


namespace LMD
{

    class Model_Stub__Voxel_Generated_Once : public LV::Builder_Stub
    {
    public:
        INIT_VARIABLE(LMD::Model_Stub__Voxel_Generated_Once, LV::Builder_Stub)

        INIT_FIELDS
        ADD_FIELD(float, voxel_size)
        FIELDS_END

        INIT_CHILDS
        ADD_CHILD("voxel_generator", voxel_generator_stub)
        ADD_CHILD("mesh_extractor", mesh_extractor_stub)
        CHILDS_END

        OVERRIDE_ON_VALUES_ASSIGNED

    public:
        float voxel_size = -1.0f;

        Voxel_3D_Generator_Stub* voxel_generator_stub = nullptr;
        Mesh_Extractor_3D_Stub* mesh_extractor_stub = nullptr;

    private:
        Mesh_3D generated_mesh;

    public:
        INIT_BUILDER_STUB(Model)

    public:
        ~Model_Stub__Voxel_Generated_Once();

    };

    class Model_Stub__Voxel_Generated_On_Construction : public LV::Builder_Stub
    {
    public:
        INIT_VARIABLE(LMD::Model_Stub__Voxel_Generated_On_Construction, LV::Builder_Stub)

        INIT_FIELDS
        ADD_FIELD(float, voxel_size)
        FIELDS_END

        INIT_CHILDS
        ADD_CHILD("voxel_generator", voxel_generator_stub)
        ADD_CHILD("mesh_extractor", mesh_extractor_stub)
        CHILDS_END

        OVERRIDE_ON_VALUES_ASSIGNED

    public:
        float voxel_size = -1.0f;

        Voxel_3D_Generator_Stub* voxel_generator_stub = nullptr;
        Mesh_Extractor_3D_Stub* mesh_extractor_stub = nullptr;

    private:
        Voxel_3D_Generator* m_voxel_generator = nullptr;
        Mesh_Extractor_3D* m_mesh_extractor = nullptr;

    public:
        INIT_BUILDER_STUB(Model)

    public:
        ~Model_Stub__Voxel_Generated_On_Construction();

    };

}
