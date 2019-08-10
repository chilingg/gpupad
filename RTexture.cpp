#include "RTexture.h"

RTexture::RTexture():
    wrapS(GL_CLAMP_TO_EDGE),
    wrapT(GL_CLAMP_TO_EDGE),
    filterMin(GL_NEAREST),
    filterMax(GL_NEAREST)
{
    glGenTextures(1, &ID);
}

RTexture::RTexture(const RImage &image):
    RTexture()
{
    generate(image);
}

RTexture::~RTexture()
{
    deleteResource();
}

bool RTexture::generate(const RImage &image)
{
    glGenTextures(1, &ID);

    state = image.isValid();
    if(!state)
        return false;

    bind();
    GLenum format;
    switch(image.channelSize())
    {
    case 4:
        format = GL_RGBA;
        break;
    case 3:
        format = GL_RGB;
        break;
    case 2:
        format = GL_RG;
        break;
    case 1:
        format = GL_RED;
        break;

    default:
        format = GL_RGBA;
        break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, image.getWidth(), image.getHeight(), 0, format, GL_UNSIGNED_BYTE, image.data());
    unBind();

    return state;
}

void RTexture::unBind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
