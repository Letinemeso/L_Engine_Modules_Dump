#pragma once

#include <Stuff/Function_Wrapper.h>

#include <Object_Constructor.h>

#include <Variable_Loader.h>

#include <Renderer/Renderer.h>


namespace LMD
{
    void register_types(LV::Object_Constructor& _object_constructor,
                        const LST::Function<const LV::Variable_Loader*()>& _resources_manager_getter,
                        LR::Renderer& _renderer,
                        LR::Draw_Order_Controller* _draw_order_controller);
}
