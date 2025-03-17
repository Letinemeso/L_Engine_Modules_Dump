#pragma once

#include <Data_Structures/Vector.h>

#include <Variable_Base.h>
#include <Builder_Stub.h>

#include <Resources/Picture.h>


namespace LMD
{

    class Animation : public LV::Variable_Base
    {
    public:
        INIT_VARIABLE(LMD::Animation, LV::Variable_Base)

    public:
        using Frame_Pictures = LDS::Vector<LR::Picture*>;

    private:
        Frame_Pictures m_frame_pictures;

    public:
        Animation();
        ~Animation();

    public:
        inline unsigned int frames_amount() const { return m_frame_pictures.size(); }
        inline const LR::Picture* get_frame(unsigned int _index) const { return m_frame_pictures[_index]; }

    public:
        void set_frame_pictures(Frame_Pictures&& _pictures);

    };


    class Animation_Stub : public LV::Builder_Stub
    {
    public:
        INIT_VARIABLE(LMD::Animation_Stub, LV::Builder_Stub)

        INIT_FIELDS
        ADD_FIELD(std::string, image_file_path)
        ADD_FIELD(unsigned int, frames_amount)
        FIELDS_END

    public:
        std::string image_file_path;
        unsigned int frames_amount = 0;

    public:
        INIT_BUILDER_STUB(Animation)

    };

}
