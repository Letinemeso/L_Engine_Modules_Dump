#include <Resources/Generated_Model.h>

using namespace LMD;


ON_VALUES_ASSIGNED_IMPLEMENTATION(Model_Stub__Voxel_Generated_Once)
{
    L_ASSERT(voxel_size > 0.0f);
    L_ASSERT(voxel_generator_stub);
    L_ASSERT(mesh_extractor_stub);

    Voxel_3D_Controller voxel_controller(voxel_size);

    Voxel_3D_Generator* voxel_generator = Voxel_3D_Generator_Stub::construct_from(voxel_generator_stub);
    Mesh_Extractor_3D* mesh_extractor = Mesh_Extractor_3D_Stub::construct_from(mesh_extractor_stub);

    voxel_generator->set_voxel_controller(&voxel_controller);
    mesh_extractor->set_voxel_controller(&voxel_controller);

    voxel_generator->generate();
    mesh_extractor->extract();

    generated_mesh = mesh_extractor->construct_combined_mesh();

    delete voxel_generator;
    delete mesh_extractor;
}



BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Model_Stub__Voxel_Generated_Once)

BUILDER_STUB_INITIALIZATION_FUNC(Model_Stub__Voxel_Generated_Once)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    L_ASSERT(generated_mesh.geometry.size() > 0);

    product->set_geometry_data(generated_mesh.geometry);
    product->set_normals_data(generated_mesh.normals);

    if(generated_mesh.texture_coordinates.size() == 0)
        return;

    product->set_texture_data(generated_mesh.texture_coordinates);
}



Model_Stub__Voxel_Generated_Once::~Model_Stub__Voxel_Generated_Once()
{
    delete voxel_generator_stub;
    delete mesh_extractor_stub;
}





ON_VALUES_ASSIGNED_IMPLEMENTATION(Model_Stub__Voxel_Generated_On_Construction)
{
    L_ASSERT(voxel_size > 0.0f);
    L_ASSERT(voxel_generator_stub);
    L_ASSERT(mesh_extractor_stub);

    m_voxel_generator = Voxel_3D_Generator_Stub::construct_from(voxel_generator_stub);
    m_mesh_extractor = Mesh_Extractor_3D_Stub::construct_from(mesh_extractor_stub);
}



BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Model_Stub__Voxel_Generated_On_Construction)

BUILDER_STUB_INITIALIZATION_FUNC(Model_Stub__Voxel_Generated_On_Construction)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    Voxel_3D_Controller voxel_controller(voxel_size);
    m_voxel_generator->set_voxel_controller(&voxel_controller);
    m_mesh_extractor->set_voxel_controller(&voxel_controller);

    m_voxel_generator->generate();
    m_mesh_extractor->extract();

    Mesh_3D generated_mesh = m_mesh_extractor->construct_combined_mesh();

    m_voxel_generator->set_voxel_controller(nullptr);
    m_mesh_extractor->set_voxel_controller(nullptr);

    L_ASSERT(generated_mesh.geometry.size() > 0);

    product->set_geometry_data( LST::move(generated_mesh.geometry) );
    product->set_normals_data( LST::move(generated_mesh.normals) );

    if(generated_mesh.texture_coordinates.size() == 0)
        return;

    product->set_texture_data( LST::move(generated_mesh.texture_coordinates) );
}



Model_Stub__Voxel_Generated_On_Construction::~Model_Stub__Voxel_Generated_On_Construction()
{
    delete voxel_generator_stub;
    delete mesh_extractor_stub;

    delete m_voxel_generator;
    delete m_mesh_extractor;
}
