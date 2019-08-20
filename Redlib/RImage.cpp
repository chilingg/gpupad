#include "RImage.h"
#include "RDebug.h"

RImage::RImage()
{

}

RImage::RImage(const char *path)
{
    load(path);
}

RImage::~RImage()
{
    deleteResource();
}

bool RImage::load(const char* path)
{
    internalData = stbi_load(path, &width, &height, &channel, 0);
    state = true;
    if(internalData)
    {
        state = false;
        RDebug() << "Texture failed to load at path: " << path;
        stbi_image_free(internalData);
    }

    return state;
}
