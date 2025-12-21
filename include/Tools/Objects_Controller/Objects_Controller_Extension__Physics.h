#pragma once

#include <Collision_Detection/Collision_Detector.h>
#include <Collision_Resolution/Collision_Resolver.h>

#include <Tools/Objects_Controller/Objects_Controller_Extension.h>


namespace LMD
{

    class Objects_Controller_Extension__Physics : public Objects_Controller_Extension
    {
    private:
        LPhys::Collision_Detector m_collision_detector;
        LPhys::Collision_Resolver m_collision_resolver;

        LST::Function<bool(LPhys::Physics_Module*)> m_registration_filter;

    public:
        inline LPhys::Collision_Detector& collision_detector() { return m_collision_detector; }
        inline LPhys::Collision_Resolver& collision_resolver() { return m_collision_resolver; }

        inline void set_registration_filter(const LST::Function<bool(LPhys::Physics_Module*)>& _func) { m_registration_filter = _func; }

    public:
        void on_object_added(LEti::Object* _object) override;
        void on_object_removed(LEti::Object* _object) override;

        void reset() override { }
        void update(Objects_List& _objects, float _dt) override;

    };

}
