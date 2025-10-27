#pragma once

#include <Draw_Modules/Draw_Module.h>
#include <Components/Graphics_Component__Default.h>

#include <Physical_Models/Physical_Model.h>


namespace LMD
{

    class Physical_Model_Renderer
    {
    private:
        LR::Draw_Module* m_draw_module = nullptr;
        LR::Graphics_Component__Default* m_coordinates_graphic_component = nullptr;

    public:
        Physical_Model_Renderer();
        ~Physical_Model_Renderer();

    public:
        inline LR::Draw_Module* draw_module() { return m_draw_module; }

    public:
        void set_draw_module(LR::Draw_Module* _ptr, unsigned int _coordinates_graphics_component_index);

    public:
        void draw(const float* _data, unsigned int _size, const LEti::Transformation_Data& _transformation_data) const;

    };

}

