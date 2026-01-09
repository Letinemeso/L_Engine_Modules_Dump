#include <LMD_Registration.h>

#include <Misc_Draw_Modules/Animation/Graphics_Component__Animation/Graphics_Component__Animation.h>
#include <Misc_Draw_Modules/Animation/Graphics_Component_Reconstructors/Animation_Graphics_Component_Reconstructor__Texture.h>
#include <Misc_Draw_Modules/Particle_Draw_Module/Draw_Module__Particle.h>
#include <Misc_Draw_Modules/Particle_Draw_Module/Shader_Components/Shader_Component__Particle_Source_Getter.h>
#include <Misc_Draw_Modules/Particle_Draw_Module/Shader_Components/Shader_Component__Update_Time.h>
#include <Misc_Draw_Modules/Particle_Draw_Module/Shader_Components/Shader_Component__Particle_Settings.h>
#include <Misc_Draw_Modules/Particle_Draw_Module/Shader_Components/Shader_Component__Initialization_Counter.h>
#include <Misc_Draw_Modules/Particle_Draw_Module/Shader_Components/Shader_Component__Update_Counter.h>
#include <Misc_Draw_Modules/Particle_Draw_Module/Shader_Components/Shader_Component__Initialization_Time.h>
#include <Misc_Draw_Modules/Particle_Draw_Module/Particle_Graphics_Component_Reconstructors/Graphics_Component_Reconstructor__Buffer_Resizer.h>
#include <Misc_Draw_Modules/Particle_Draw_Module/Graphics_Components/Graphics_Component__Particles_Amount_Reader.h>
#include <Misc_Draw_Modules/Particle_System/Particle_System_Module.h>
#include <Misc_Draw_Modules/Text_Field/Draw_Module__Text_Field.h>
#include <Misc_Draw_Modules/Text_Field/Graphics_Component_Reconstructors/TF_Graphics_Component_Reconstructor__Coordinates.h>
#include <Misc_Draw_Modules/Text_Field/Graphics_Component_Reconstructors/TF_Graphics_Component_Reconstructor__Colors.h>
#include <Misc_Draw_Modules/Text_Field/Graphics_Component_Reconstructors/TF_Graphics_Component_Reconstructor__Texture.h>
#include <Misc_Draw_Modules/Utility_Reconstructors/Graphics_Component_Reconstructor__Filler.h>
#include <Misc_Draw_Modules/Utility_Draw_Modules/Static_Draw_Module.h>
#include <Misc_Draw_Modules/Utility_Draw_Modules/Draw_Module_Storage.h>
#include <Misc_Draw_Modules/Vertex_Animation/Draw_Module__Vertex_Animation.h>
#include <Misc_Draw_Modules/Vertex_Animation/Shader_Component__Vertex_Animation.h>
#include <Misc_Modules/Follow_Trajectory_Module.h>
#include <Misc_Modules/Following_Sound_Module.h>
#include <Misc_Modules/Command_Queue_Module.h>
#include <Misc_Modules/Timer_Module.h>
#include <Misc_Modules/Module_Container.h>
#include <Misc_Modules/Directional_Movement_Module.h>
#include <Misc_Modules/Rigid_Body_Setup_Module.h>
#include <Resources/Animation.h>
#include <Resources/Model.h>
#include <Resources/Vertex_Animation.h>
#include <Tools/Objects_Storage.h>
#include <Tools/Model_Data_Provider.h>
#include <Tools/Objects_Controller/Objects_Controller_Extension__Removal.h>
#include <Physics/Rigid_Body/Physics_Module__Rigid_Body.h>

using namespace LMD;


void LMD::register_types(LV::Object_Constructor& _object_constructor,
                         const LST::Function<const LV::Variable_Loader*()>& _resources_manager_getter,
                         LR::Renderer& _renderer,
                         LR::Draw_Order_Controller* _draw_order_controller)
{
    LST::Message_Translator::instance().register_message_type<Message__Request_Object_Deletion>();


    L_ASSERT(_resources_manager_getter);

    LR::Renderer* renderer = &_renderer;

    _object_constructor.register_type<LMD::Graphics_Component_Stub__Animation>().override_initialization_func([_resources_manager_getter](LV::Variable_Base* _product)
    {
        LMD::Graphics_Component_Stub__Animation* product = (LMD::Graphics_Component_Stub__Animation*)_product;

        product->resources_manager = _resources_manager_getter();
    });

    _object_constructor.register_type<LMD::Animation_Graphics_Component_Reconstructor_Stub__Texture>();

    _object_constructor.register_type<LMD::Draw_Module_Stub__Particle>();

    _object_constructor.register_type<LMD::Shader_Component_Stub__Particle_Source_Getter>();

    _object_constructor.register_type<LMD::Shader_Component_Stub__Update_Time>();

    _object_constructor.register_type<LMD::Shader_Component_Stub__Particle_Settings>();

    _object_constructor.register_type<LMD::Shader_Component_Stub__Initialization_Counter>();

    _object_constructor.register_type<LMD::Shader_Component_Stub__Update_Counter>();

    _object_constructor.register_type<LMD::Shader_Component_Stub__Initialization_Time>();

    _object_constructor.register_type<LMD::Graphics_Component_Reconstructor_Stub__Buffer_Resizer>();

    _object_constructor.register_type<LMD::Graphics_Component_Stub__Particles_Amount_Reader>();

    _object_constructor.register_type<Particle_System::Particle_System_Module_Stub>().override_initialization_func([_draw_order_controller](LV::Variable_Base* _product)
    {
        Particle_System::Particle_System_Module_Stub* product = (Particle_System::Particle_System_Module_Stub*)_product;

        product->draw_order_controller = _draw_order_controller;
    });

    _object_constructor.register_type<LMD::Draw_Module_Stub__Text_Field>().override_initialization_func([renderer, _resources_manager_getter](LV::Variable_Base* _product)
    {
        LMD::Draw_Module_Stub__Text_Field* product = (LMD::Draw_Module_Stub__Text_Field*)_product;

        product->renderer = renderer;
        product->resources_manager = _resources_manager_getter();
    });

    _object_constructor.register_type<LMD::TF_Graphics_Component_Reconstructor_Stub__Coordinates>();

    _object_constructor.register_type<LMD::TF_Graphics_Component_Reconstructor_Stub__Colors>();

    _object_constructor.register_type<LMD::TF_Graphics_Component_Reconstructor_Stub__Texture>();

    _object_constructor.register_type<LMD::Graphics_Component_Reconstructor_Stub__Filler>();

    _object_constructor.register_type<LMD::Static_Draw_Module_Stub>();

    _object_constructor.register_type<LMD::Draw_Module_Storage_Stub>();

    _object_constructor.register_type<LMD::Draw_Module_Stub__Vertex_Animation>().override_initialization_func([_resources_manager_getter](LV::Variable_Base* _product)
    {
        LMD::Draw_Module_Stub__Vertex_Animation* product = (LMD::Draw_Module_Stub__Vertex_Animation*)_product;

        product->resources_manager = _resources_manager_getter();
    });

    _object_constructor.register_type<LMD::Shader_Component_Stub__Vertex_Animation>();

    _object_constructor.register_type<LMD::Follow_Trajectory_Module_Stub>();

    _object_constructor.register_type<LMD::Following_Sound_Module_Stub>();

    _object_constructor.register_type<LMD::Command_Queue_Module_Stub>();

    _object_constructor.register_type<LMD::Timer_Module_Stub>();

    _object_constructor.register_type<LMD::Module_Container_Stub>();

    _object_constructor.register_type<LMD::Directional_Movement_Module_Stub>();

    _object_constructor.register_type<LMD::Rigid_Body_Setup_Module_Stub>();

    _object_constructor.register_type<LMD::Animation_Stub>();

    _object_constructor.register_type<LMD::Model_Stub>();

    _object_constructor.register_type<LMD::Vertex_Animation_Stub>();

    _object_constructor.register_type<LMD::Objects_Storage>();

    _object_constructor.register_type<LMD::Model_Data_Provider>().override_initialization_func([_resources_manager_getter](LV::Variable_Base* _product)
    {
        LMD::Model_Data_Provider* product = (LMD::Model_Data_Provider*)_product;

        product->inject_resources_manager(_resources_manager_getter());
    });

    _object_constructor.register_type<LMD::Physics_Module_Stub__Rigid_Body>();
}
