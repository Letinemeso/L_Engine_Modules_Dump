#pragma once

#include <Data_Structures/List.h>

#include <Object.h>


namespace LMD
{

    class Objects_Controller_Extension
    {
    protected:
        using Objects_List = LDS::List<LEti::Object*>;

    public:
        Objects_Controller_Extension() { }
        virtual ~Objects_Controller_Extension() { }

    public:
        virtual void on_object_added(LEti::Object* _object) = 0;
        virtual void on_object_removed(LEti::Object* _object) = 0;

        virtual void reset() = 0;
        virtual void update(Objects_List& _objects, float _dt) = 0;

        virtual bool should_remove_object(LEti::Object* _object) { return false; }

    };

}
