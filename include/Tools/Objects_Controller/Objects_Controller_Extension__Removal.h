#pragma once

#include <Stuff/Message_Type.h>
#include <Stuff/Message_Translator.h>

#include <Tools/Objects_Controller/Objects_Controller_Extension.h>


namespace LMD
{

    MESSAGE_TYPE_DECLARATION_START(Message__Request_Object_Deletion)
    LEti::Object* object = nullptr;
    MESSAGE_TYPE_DECLARATION_END


    class Objects_Controller_Extension__Removal : public Objects_Controller_Extension
    {
    private:
        LST::Message_Translator::Handle<Message__Request_Object_Deletion> m_message_handle;

    private:
        using Entity_Set = LDS::AVL_Tree<LEti::Object*>;
        Entity_Set m_should_be_dead_entities;

    public:
        Objects_Controller_Extension__Removal();
        ~Objects_Controller_Extension__Removal();

    public:
        void on_object_added(LEti::Object* _object) override { }
        void on_object_removed(LEti::Object* _object) override { }

        void reset() override;
        void update(Objects_List& _objects, float _dt) override { }

        bool should_remove_object(LEti::Object* _object) override;

    };

}
