#pragma once

#include <Stuff/Function_Wrapper.h>

#include <Module.h>


namespace LMD
{

    class Custom_Module : public LEti::Module
    {
    public:
        INIT_VARIABLE(LMD::Custom_Module, LEti::Module)

    public:
        using Update_Function = LST::Function<void(LEti::Object*, float)>;

    private:
        Update_Function m_update_function;

    public:
        inline void set_update_function(const Update_Function& _func) { m_update_function = _func; }

    public:
        void update(float _dt) override;

    };

}
