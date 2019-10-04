#include "RTexture.h"

RTexture::RTexture()
{
    glGenTextures(1, &_ID);
}

RTexture::RTexture(const RImage &image):
    RTexture()
{
    generate(image);
}

RTexture::RTexture(const RTexture &tex):
    _ID( tex._ID),
    _width(tex._width),
    _height(tex._height)
{
    xxxxx
}

RTexture::~RTexture()
{
    deleteResource();
}

bool RTexture::generate(const RImage &image)
{
    return generate(image.getWidth(), image.getHeight(), image.data(), image.channelSize());
}

bool RTexture::generate(int width, int height, const unsigned char* data, int channel)
{
    glGenTextures(1, &_ID);
    glBindTexture(GL_TEXTURE_2D, _ID);
    float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMax);

    if(!data)
    {
        state = false;
        return false;
    }
    state = true;

    _width = width;
    _height = height;

    bind();
    GLenum format;
    switch(channel)
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

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    unBind();

    return state;
}
