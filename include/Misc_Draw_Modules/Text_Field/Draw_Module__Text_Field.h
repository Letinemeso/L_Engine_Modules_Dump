#pragma once

#include <Math_Stuff.h>

#include <Draw_Modules/Draw_Module.h>
#include <Picture/Font.h>
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
        LR::Graphics_Component* m_coords = nullptr;
        LR::Graphics_Component* m_colors = nullptr;
        LR::Graphics_Component__Texture* m_texture = nullptr;

    private:
        Text_Field_Settings m_configurable_settings;    //  settings, configurable from outside of object
        Text_Field_Settings m_current_settings;         //  settings for current configuration

    public:
        inline void set_coords_component(LR::Graphics_Component* _ptr) { m_coords = _ptr; }
        inline void set_colors_component(LR::Graphics_Component* _ptr) { m_colors = _ptr; }
        inline void set_texture_component(LR::Graphics_Component__Texture* _ptr) { m_texture = _ptr; }

    public:
        inline Text_Field_Settings& settings() { return m_configurable_settings; }
        inline const Text_Field_Settings& settings() const { return m_configurable_settings; }

    private:
        glm::vec2 M_calculate_raw_size() const;
        float M_calculate_raw_scale(const glm::vec2& _raw_size) const;

        void M_construct_coords(float* _coords, unsigned int _amount, unsigned int _amount_per_character);
        void M_construct_colors(float* _colors, unsigned int _amount);
        void M_construct_texture_coords(float* _texture_coords, unsigned int _amount, unsigned int _amount_per_character);

        void M_reconfigure();

    public:
        void update(float _dt) override;

    };



    class Draw_Module_Stub__Text_Field : public LR::Draw_Module_Stub
    {
    public:
        INIT_VARIABLE(LMD::Draw_Module_Stub__Text_Field, LR::Draw_Module_Stub)

        INIT_FIELDS
        ADD_FIELD(std::string, font_name)
        ADD_FIELD(unsigned int, horizontal_alignment)
        ADD_FIELD(unsigned int, vertical_alignment)
        ADD_FIELD(glm::vec3, raw_offset)
        ADD_FIELD(float, raw_size_multiplier)
        ADD_FIELD(float, raw_size)
        ADD_FIELD(std::string, text)
        FIELDS_END

        INIT_CHILDS
        ADD_CHILD("TF_Required__Graphics_Component_Stub__coords", coords_stub)
        ADD_CHILD("TF_Required__Graphics_Component_Stub__colors", colors_stub)
        ADD_CHILD("TF_Required__Graphics_Component_Stub__texture", texture_stub)
        CHILDS_END


    public:
        std::string font_name;
        unsigned int horizontal_alignment = 0;
        unsigned int vertical_alignment = 0;
        glm::vec3 raw_offset{0.0f, 0.0f, 0.0f};
        float raw_size_multiplier = 1.0f;
        float raw_size = -1.0f;     //  negative value means no limit
        std::string text;

    public:
        LR::Graphics_Component_Stub* coords_stub = nullptr;
        LR::Graphics_Component_Stub* colors_stub = nullptr;
        LR::Graphics_Component_Stub__Texture* texture_stub = nullptr;

    public:
        ~Draw_Module_Stub__Text_Field();

    protected:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* _product) const override;

    };

}
