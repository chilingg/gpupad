#include "RImage.h"
#include "RDebug.h"

RImage::RImage()
{

}

RImage::RImage(const char *path, bool flip)
{
    load(path, flip);
}

RImage::RImage(const std::string &path, bool flip):
    RImage(path.c_str(), flip)
{

}

RImage::~RImage()
{
    deleteResource();
}

bool RImage::load(const char* path, bool flip)
{
    stbi_set_flip_vertically_on_load(flip);
    internalData = stbi_load(path, &width, &height, &channel, 0);
    if(flip)
        stbi_set_flip_vertically_on_load(false);
    state = true;
    if(!internalData)
    {
        state = false;
        printErro("Image failed to load at path: ");
        printErro(path);
        stbi_image_free(internalData);
    }

    return state;
}
