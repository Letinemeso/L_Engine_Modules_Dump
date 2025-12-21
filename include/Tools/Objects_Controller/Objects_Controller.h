#pragma once

#include <Data_Structures/List.h>
#include <Data_Structures/AVL_Tree.h>

#include <Object.h>

#include <Tools/Objects_Controller/Objects_Controller_Extension.h>


namespace LMD
{

    class Objects_Controller
    {
    public:
        using Objects_List = LDS::List<LEti::Object*>;
        using Extensions = LDS::Vector<Objects_Controller_Extension*>;

    private:
        Objects_List m_objects;
        Extensions m_extensions;

    public:
        Objects_Controller();
        ~Objects_Controller();

    public:
        inline void add_extension(Objects_Controller_Extension* _ptr) { m_extensions.push(_ptr); }

    public:
        inline const Objects_List& objects() const { return m_objects; }

        LEti::Object* get_suitable_object(const LST::Function<bool(LEti::Object*)>& _condition) const;

    public:
        void add_object(LEti::Object* _object);         //  takes ownership
        void remove_object(LEti::Object* _object);     //  entity will be deleted during next update
        void clear_objects();
        void clear_extensions();

    private:
        bool M_need_to_remove_object(LEti::Object* _object) const;
        void M_remove_objects();

    public:
        void update_previous_state();
        void update(float _dt);

    };

}
