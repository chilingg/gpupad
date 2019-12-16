#include "RTexture.h"

#include "RDebug.h"

void RTexture::hintWrapPattern(RTexture::TextureWrap s, RTexture::TextureWrap t)
{
    wrapS = s;
    wrapT = t;
}

void RTexture::hintTexFilter(RTexture::TextureFilter mag, RTexture::TextureFilter min)
{
    filterMax = mag;
    filterMin = min;
}

RTexture::RTexture():
    RResource("UnnameTexture")
{

}

RTexture::RTexture(const RImage &img, const std::string &name):
    RResource(name)
{
    generate(img.width(), img.height(), img.channel(), img.cdata(), img.channel());
}

RTexture::RTexture(const RTexture &texture):
    RResource(texture),
    textureID_(texture.textureID_),
    wrapS_(texture.wrapS_),
    wrapT_(texture.wrapT_),
    filterMin_(texture.filterMin_),
    filterMax_(texture.filterMax_),
    borderColor_{texture.borderColor_[0],
                texture.borderColor_[1],
                texture.borderColor_[2],
                texture.borderColor_[3],},
    width_(texture.width_),
    height_(texture.height_)
{

}

RTexture::RTexture(const RTexture &&texture):
    RResource(texture),
    textureID_(texture.textureID_),
    wrapS_(texture.wrapS_),
    wrapT_(texture.wrapT_),
    filterMin_(texture.filterMin_),
    filterMax_(texture.filterMax_),
    borderColor_{texture.borderColor_[0],
                texture.borderColor_[1],
                texture.borderColor_[2],
                texture.borderColor_[3],},
    width_(texture.width_),
    height_(texture.height_)
{

}

RTexture &RTexture::operator=(RTexture texture)
{
    swap(texture);
    return *this;
}

void RTexture::swap(RTexture &texture)
{
    RResource::swap(texture);
    using std::swap;
    swap(textureID_, texture.textureID_);
    swap(wrapS_, texture.wrapS_);
    swap(wrapT_, texture.wrapT_);
    swap(filterMin_, texture.filterMin_);
    swap(filterMax_, texture.filterMax_);
    swap(borderColor_, texture.borderColor_);
    swap(width_, texture.width_);
    swap(height_, texture.height_);
}

RTexture::~RTexture()
{

}

void RTexture::setWrapPattern(RTexture::TextureWrap s, RTexture::TextureWrap t)
{
    wrapS_ = s;
    wrapT_ = t;
}

void RTexture::setTexFilter(RTexture::TextureFilter mag, RTexture::TextureFilter min)
{
    filterMax_ = mag;
    filterMin_ = min;
}

bool RTexture::generate(int width, int height, int echannel, const unsigned char *data, int ichannel)
{
#ifdef R_DEBUG
    if(!data)
    {
        printError("Generate Texture data from nullptr! In " + nameID());
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
        printError("Invalid of image channels! In " + nameID());
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
        printError("Invalid of texture channels! In " + nameID());
        return false;
    }

    GLuint *ID = new GLuint;
    glGenTextures(1, ID);
    textureID_.reset(ID, deleteTexture);

    glBindTexture(GL_TEXTURE_2D, *ID);
    glTexParameterIuiv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor_);
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
        printError("Generate Texture data from invalid image! In " + nameID());
        return false;
    }
#endif
    return generate(img.width(), img.height(), img.channel(), img.cdata(), img.channel());
}

bool RTexture::generate(const std::string path)
{
    RImage img;
    img.load(path, true);
    return generate(img);
}

void RTexture::freeTexture()
{
    textureID_.reset();
}

void RTexture::bind(GLenum unit)
{
#ifdef R_DEBUG
    if(!textureID_)
    {
        printError("Binding invalid texture for " + nameID());
        return;
    }
#endif

    glActiveTexture(unit + TEXTURE_UNIT);
    glBindTexture(GL_TEXTURE_2D, *textureID_);
}

void RTexture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool RTexture::isValid() const
{
    return textureID_ != nullptr;
}

int RTexture::width() const
{
    return width_;
}

int RTexture::height() const
{
    return height_;
}

RSize RTexture::size() const
{
    return {width_, height_};
}

void RTexture::deleteTexture(GLuint *ID)
{
    glDeleteTextures(1, ID);
    delete ID;
}

void swap(RTexture &texture1, RTexture &texture2)
{
    texture1.swap(texture2);
}

GLint RTexture::wrapS = GL_CLAMP_TO_BORDER;
GLint RTexture::wrapT = GL_CLAMP_TO_BORDER;
GLint RTexture::filterMin = GL_NEAREST;
GLint RTexture::filterMax = GL_LINEAR;
GLuint RTexture::borderColor[4]{ 0, 0, 0, 255 };
