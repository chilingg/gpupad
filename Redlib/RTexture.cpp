#include "RTexture.h"

RTexture::RTexture():
    RResource(),
    _ID(std::make_shared<GLuint>(0))
{
}

RTexture::RTexture(const RImage &image):
    RTexture()
{
    generate(image);
}

RTexture &RTexture::operator=(const RTexture &tex)
{
    //RResource::operator=(tex);
    RTexture temp(*this);//如果temp没有贡献贡献共享 析构函数负责删除

    _ID = tex._ID;
    wrapS = tex.wrapS;
    wrapT = tex.wrapT;
    filterMin = tex.filterMin;
    filterMax = tex.filterMax;
    _width = tex._width;
    _height = tex._height;

    return *this;
}

RTexture::~RTexture()
{
    if(_ID.unique() && *_ID != 0)
        glDeleteTextures(1, _ID.get());
}

bool RTexture::generate(const RImage &image)
{
    return generate(image.getWidth(), image.getHeight(), image.data(), image.channelSize());
}

bool RTexture::generate(int width, int height, const unsigned char* data, int channel)
{
    if(!data)
        return false;

    if(!_ID.unique())
        _ID = std::make_shared<GLuint>(0);
    else if(*_ID)
        glDeleteTextures(1, _ID.get());

    glGenTextures(1, _ID.get());
    glBindTexture(GL_TEXTURE_2D, *_ID);
    float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMax);

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

    return *_ID;
}
