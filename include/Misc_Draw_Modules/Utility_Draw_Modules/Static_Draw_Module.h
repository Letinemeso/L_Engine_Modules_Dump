#pragma once

#include <Draw_Modules/Draw_Module.h>


namespace LMD
{

    class Static_Draw_Module : public LR::Draw_Module
    {
    public:
        INIT_VARIABLE(LMD::Static_Draw_Module, LR::Draw_Module)

    private:
        LEti::Transformation_Data* m_parent_transformation_data = nullptr;
        const LEti::Transformation_Data* m_parent_transformation_data_prev_state = nullptr;

        LEti::Transformation_Data m_initial_transformation;

    public:
        inline const LEti::Transformation_Data* parent_transformation_data() const { return m_parent_transformation_data; }
        inline const LEti::Transformation_Data* parent_transformation_data_prev_state() const { return m_parent_transformation_data; }

    public:
        void set_transformation_data(LEti::Transformation_Data* _data) override;
        void set_transformation_data_prev_state(const LEti::Transformation_Data* _data) override;

    };


    class Static_Draw_Module_Stub : public LR::Draw_Module_Stub
    {
    public:
        INIT_VARIABLE(LMD::Static_Draw_Module_Stub, LR::Draw_Module_Stub)

    public:
        INIT_BUILDER_STUB(Static_Draw_Module)

    };

}
