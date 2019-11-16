#include "RTexture.h"

#include "RDebug.h"

RTexture::RTexture():
    RResource()
{

}

RTexture::RTexture(const RImage &img):
    RTexture()
{
    generate(img.width(), img.height(), img.channel(), img.cdata(), img.channel());
}

RTexture::~RTexture()
{

}

void RTexture::setWrapPattern(RTexture::TextureWrap s, RTexture::TextureWrap t)
{
    wrapS_ = s;
    wrapT_ = t;
}

bool RTexture::generate(int width, int height, int echannel, const unsigned char *data, int ichannel)
{
#ifdef R_DEBUG
    if(!data)
    {
        printError("Generate Texture data from nullptr!");
        return false;
    }
#endif

    GLint eformat = RGBA;
    switch(echannel){
    case 4:
        eformat = RGBA; break;
    case 3:
        eformat = RGB; break;
    case 2:
        eformat = RG; break;
    case 1:
        eformat = RED; break;
    default:
        printError("Invalid of image channels!");
        return false;
    }
    GLuint iformat = RGBA8;
    switch(ichannel){
    case 4:
        iformat = RGBA8; break;
    case 3:
        iformat = RGB8; break;
    case 2:
        iformat = RG8; break;
    case 1:
        iformat = R8; break;
    default:
        printError("Invalid of texture channels!");
        return false;
    }

    GLuint *ID = new GLuint;
    glGenTextures(1, ID);
    ID_.reset(ID, deleteTexture);

    glBindTexture(GL_TEXTURE_2D, *ID);
    glTexParameterIuiv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMax_);

    glTexImage2D(GL_TEXTURE_2D, 0, iformat, width, height, 0, eformat, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    width_ = width;
    height_ = height;

    return glIsTexture(*ID);
}

bool RTexture::generate(const RImage &img)
{
#ifdef R_DEBUG
    if(!img.isValid())
    {
        printError("Generate Texture data from invalid image!");
        return false;
    }
#endif
    return generate(img.width(), img.height(), img.channel(), img.cdata(), img.channel());
}

void RTexture::freeTexture()
{
    ID_.reset();
}

void RTexture::bind(GLenum unit)
{
#ifdef R_DEBUG
    if(!ID_)
    {
        printError("Binding invalid texture!");
        return;
    }
#endif

    glActiveTexture(unit + TEXTURE_UNIT);
    glBindTexture(GL_TEXTURE_2D, *ID_);
}

void RTexture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool RTexture::isValid() const
{
    return ID_ != nullptr;
}

int RTexture::width() const
{
    return width_;
}

int RTexture::height() const
{
    return height_;
}

void RTexture::deleteTexture(GLuint *ID)
{
    glDeleteTextures(1, ID);
    delete ID;
}
