#include <LMD_Registration.h>

#include <Misc_Draw_Modules/Animation/Graphics_Component__Animation/Graphics_Component__Animation.h>
#include <Misc_Draw_Modules/Animation/Graphics_Component_Reconstructors/Animation_Graphics_Component_Reconstructor__Texture.h>
#include <Misc_Draw_Modules/Particle_Draw_Module/Draw_Module__Particle.h>
#include <Misc_Draw_Modules/Particle_Draw_Module/Particle_Graphics_Component_Reconstructors/Graphics_Component_Reconstructor__Particle.h>
#include <Misc_Draw_Modules/Particle_Draw_Module/Particle_Graphics_Component_Reconstructors/Graphics_Component_Reconstructor__Particle_Position_Setter.h>
#include <Misc_Draw_Modules/Particle_System/Particle_System_Module.h>
#include <Misc_Draw_Modules/Text_Field/Draw_Module__Text_Field.h>
#include <Misc_Draw_Modules/Text_Field/Graphics_Component_Reconstructors/TF_Graphics_Component_Reconstructor__Coordinates.h>
#include <Misc_Draw_Modules/Text_Field/Graphics_Component_Reconstructors/TF_Graphics_Component_Reconstructor__Colors.h>
#include <Misc_Draw_Modules/Text_Field/Graphics_Component_Reconstructors/TF_Graphics_Component_Reconstructor__Texture.h>
#include <Misc_Modules/Follow_Trajectory_Module.h>
#include <Resources/Animation.h>

using namespace LMD;


void LMD::register_types(LV::Object_Constructor& _object_constructor,
                         const LST::Function<const LEti::Resources_Manager*()>& _resources_manager_getter,
                         LR::Renderer& _renderer,
                         LR::Draw_Order_Controller* _draw_order_controller)
{
    L_ASSERT(_resources_manager_getter);

    LR::Renderer* renderer = &_renderer;

    _object_constructor.register_type<LMD::Graphics_Component_Stub__Animation>().override_initialization_func([_resources_manager_getter](LV::Variable_Base* _product)
    {
        LMD::Graphics_Component_Stub__Animation* product = (LMD::Graphics_Component_Stub__Animation*)_product;

        product->resources_manager = _resources_manager_getter();
    });

    _object_constructor.register_type<LMD::Animation_Graphics_Component_Reconstructor_Stub__Texture>();

    _object_constructor.register_type<LMD::Draw_Module_Stub__Particle>();

    _object_constructor.register_type<LMD::Graphics_Component_Reconstructor_Stub__Particle>();

    _object_constructor.register_type<LMD::Graphics_Component_Reconstructor_Stub__Particle_Position_Setter>();

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

    _object_constructor.register_type<LMD::Follow_Trajectory_Module_Stub>();

    _object_constructor.register_type<LMD::Animation_Stub>();
}
