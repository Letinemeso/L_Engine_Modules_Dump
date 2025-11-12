#include <Resources/Vertex_Animation.h>

using namespace LMD;


ON_VALUES_ASSIGNED_IMPLEMENTATION(Vertex_Animation_Stub)
{
    frames_amount = frames.size();
    L_ASSERT(frames_amount > 0);

    LDS::Vector<Model*> frame_models(frames_amount);
    for(LV::Variable_Base::Childs_List::Iterator it = frames.begin(); !it.end_reached(); ++it)
    {
        L_ASSERT(LV::cast_variable<Model_Stub>(it->child_ptr));
        Model_Stub* stub = (Model_Stub*)it->child_ptr;

        frame_models.push(Model_Stub::construct_from(stub));
    }

    vertices_per_frame = frame_models[0]->geometry_data().size() / 3;

    unsigned int geometry_data_size = frame_models[0]->geometry_data().size();
    unsigned int texture_data_size = frame_models[0]->texture_data().size();
    unsigned int normals_data_size = frame_models[0]->normals_data().size();

    L_DEBUG_FUNC_NOARG([&]()
    {
        for(unsigned int i = 1; i < frames_amount; ++i)
        {
            L_ASSERT(frame_models[i]->geometry_data().size() == geometry_data_size);
            L_ASSERT(frame_models[i]->texture_data().size() == texture_data_size);
            L_ASSERT(frame_models[i]->normals_data().size() == normals_data_size);
        }
    });

    geometry_data.mark_empty();
    geometry_data.resize(geometry_data_size * frames_amount);
    texture_data.mark_empty();
    texture_data.resize(texture_data_size * frames_amount);
    normals_data.mark_empty();
    normals_data.resize(normals_data_size * frames_amount);

    for(unsigned int frame_i = 0; frame_i < frames_amount; ++frame_i)
    {
        const LDS::Vector<float>& geometry = frame_models[frame_i]->geometry_data();
        const LDS::Vector<float>& texture = frame_models[frame_i]->texture_data();
        const LDS::Vector<float>& normals = frame_models[frame_i]->normals_data();

        for(unsigned int i = 0; i < geometry_data_size; ++i)
            geometry_data.push(geometry[i]);
        for(unsigned int i = 0; i < texture_data_size; ++i)
            texture_data.push(texture[i]);
        for(unsigned int i = 0; i < normals_data_size; ++i)
            normals_data.push(normals[i]);

        delete frame_models[frame_i];
    }
}



BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Vertex_Animation_Stub)

BUILDER_STUB_INITIALIZATION_FUNC(Vertex_Animation_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    L_ASSERT(frames_amount > 0);
    L_ASSERT(vertices_per_frame > 0);
    L_ASSERT(geometry_data.size() > 0);
    L_ASSERT(normals_data.size() > 0);

    product->set_frames_amount(frames_amount);
    product->set_vertices_per_frame(vertices_per_frame);
    product->set_geometry_data(geometry_data);
    product->set_texture_data(texture_data);
    product->set_normals_data(normals_data);
}



Vertex_Animation_Stub::~Vertex_Animation_Stub()
{
    clear_childs_list(frames);
}
