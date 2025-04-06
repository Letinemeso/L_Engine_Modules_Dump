#pragma once

#include <Stuff/Function_Wrapper.h>

#include <Object_Constructor.h>

#include <Resources_Manager.h>

#include <Renderer/Renderer.h>


namespace LMD
{
    void register_types(LV::Object_Constructor& _object_constructor,
                        const LST::Function<const LEti::Resources_Manager*()>& _resources_manager_getter,
                        LR::Renderer& _renderer);
}
