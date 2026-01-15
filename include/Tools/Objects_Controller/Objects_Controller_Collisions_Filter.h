#pragma once

#include <Collision_Detection/Collision_Detector.h>


namespace LMD
{

    class Objects_Controller_Collisions_Filter
    {
    public:
        Objects_Controller_Collisions_Filter() { }
        virtual ~Objects_Controller_Collisions_Filter() { }

    public:
        virtual LPhys::Collision_Detector::Intersection_Data_List filter(const LPhys::Collision_Detector::Intersection_Data_List& _initial_list) = 0;

    };

}
