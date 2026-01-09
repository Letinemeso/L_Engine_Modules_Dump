#pragma once

#include <Stuff/Math_Stuff.h>

#include <Resources_Manager.h>

#include <Resources/Font.h>
#include <Resources/Picture.h>
#include <Draw_Modules/Draw_Module.h>
#include <Components/Graphics_Component__Texture.h>


namespace LMD
{

    class Text_Field_Settings
    {
    public:
        enum class Horizontal_Alignment
        {
            Left = 0,
            Right = 1,
            Center = 2
        };
        enum class Vertical_Alignment
        {
            Bottom = 0,
            Top = 1,
            Center = 2
        };

        const LR::Font* font = nullptr;
        const LR::Picture* picture = nullptr;

        Horizontal_Alignment horizontal_alignment = Horizontal_Alignment::Left;
        Vertical_Alignment vertical_alignment = Vertical_Alignment::Bottom;

        glm::vec3 raw_offset{0.0f, 0.0f, 0.0f};
        float raw_size_multiplier = 1.0f;
        //  1.0f means whole string will fit inside 1 x 1 square ( hope it makes sence ). in other words, proportionally shrinks/stretches result string so width or height are not larger then raw_size...
        float raw_size = 1.0f;

        std::string text;

    public:
        bool operator==(const Text_Field_Settings& _other) const;
        bool operator!=(const Text_Field_Settings& _other) const;

    };


    class Draw_Module__Text_Field : public LR::Draw_Module
    {
    public:
        INIT_VARIABLE(LMD::Draw_Module__Text_Field, LR::Draw_Module)

    private:
        //      !!!   if nothing is being drawn, check if draw module's components have correct reconstructors   !!!

    private:
        Text_Field_Settings m_configurable_settings;    //  settings, configurable from outside of object
        Text_Field_Settings m_current_settings;         //  settings for current configuration

    public:
        inline Text_Field_Settings& settings() { return m_configurable_settings; }
        inline const Text_Field_Settings& settings() const { return m_configurable_settings; }
        inline const Text_Field_Settings& current_settings() const { return m_current_settings; }

    protected:
        void M_update_internal(float _dt) override;
        void draw() const override;

    };


    class Draw_Module_Stub__Text_Field : public LR::Draw_Module_Stub
    {
    public:
        INIT_VARIABLE(LMD::Draw_Module_Stub__Text_Field, LR::Draw_Module_Stub)

        INIT_FIELDS
        ADD_FIELD(std::string, font_name)
        ADD_FIELD(std::string, picture_name)
        ADD_FIELD(unsigned int, horizontal_alignment)
        ADD_FIELD(unsigned int, vertical_alignment)
        ADD_FIELD(glm::vec3, raw_offset)
        ADD_FIELD(float, raw_size_multiplier)
        ADD_FIELD(float, raw_size)
        ADD_FIELD(std::string, text)
        FIELDS_END

    public:
        const LEti::Resources_Manager* resources_manager = nullptr;

    public:
        std::string font_name;
        std::string picture_name;
        unsigned int horizontal_alignment = 0;
        unsigned int vertical_alignment = 0;
        glm::vec3 raw_offset{0.0f, 0.0f, 0.0f};
        float raw_size_multiplier = 1.0f;
        float raw_size = -1.0f;     //  negative value means no limit
        std::string text;

    protected:
        INIT_BUILDER_STUB(Draw_Module__Text_Field)

    };

}
