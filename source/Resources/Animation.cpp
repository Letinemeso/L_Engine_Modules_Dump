#include <Resources/Animation.h>

using namespace LMD;


Animation::Animation()
{

}

Animation::~Animation()
{
    for(unsigned int i = 0; i < m_frame_pictures.size(); ++i)
        delete m_frame_pictures[i];
}



void Animation::set_frame_pictures(Frame_Pictures&& _pictures)
{
    for(unsigned int i = 0; i < m_frame_pictures.size(); ++i)
        delete m_frame_pictures[i];

    m_frame_pictures = (Frame_Pictures&&)_pictures;
}





BUILDER_STUB_DEFAULT_CONSTRUCTION_FUNC(Animation_Stub)

BUILDER_STUB_INITIALIZATION_FUNC(Animation_Stub)
{
    BUILDER_STUB_PARENT_INITIALIZATION;
    BUILDER_STUB_CAST_PRODUCT;

    L_ASSERT(frames_amount > 0);
    L_ASSERT(image_file_path.size() > 0);

    LR::Picture_Stub picture_stub;
    picture_stub.image_file_path = image_file_path;
    LR::Picture* loaded_picture = LR::Picture_Stub::cast_product(picture_stub.construct());

    unsigned int frame_width = loaded_picture->width() / frames_amount;
    unsigned int frame_height = loaded_picture->height();

    Animation::Frame_Pictures frames(frames_amount, nullptr);

    glm::vec4 default_color_multiplier = {1.0f, 1.0f, 1.0f, 1.0f};
    for(unsigned int i = 0; i < frames_amount; ++i)
    {
        frames[i] = new LR::Picture(frame_width, frame_height, loaded_picture->has_alpha());
        frames[i]->replace_with_picture(0, 0, loaded_picture, default_color_multiplier, frame_width * i, 0, frame_width, frame_height);
    }

    delete loaded_picture;

    product->set_frame_pictures((Animation::Frame_Pictures&&)frames);
}
