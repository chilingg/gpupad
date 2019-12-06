#ifndef RTEXTURE_H
#define RTEXTURE_H

#include "RResource.h"
#include "RImage.h"
#include "ROpenGL.h"
#include "RSize.h"

#include <memory>

class RTexture : public RResource
{
    friend void swap(RTexture &texture1, RTexture &texture2);

public:
    enum Format {
        R8 = GL_R8,
        RG8 = GL_RG8,
        RGB8 = GL_RGB8,
        RGBA8 = GL_RGBA8,
        UKNOW_FORMAT
    };
    enum ExternalFormat {
        RED = GL_RED,
        RG = GL_RG,
        RGB = GL_RGB,
        RGBA = GL_RGBA,
        UKNOW_EFORMAT
    };
    enum TextureWrap {
        Repeat = GL_REPEAT,
        MirroredRepeat = GL_MIRRORED_REPEAT,
        ClampToEdge = GL_CLAMP_TO_EDGE,
        ClampToBorder = GL_CLAMP_TO_BORDER
    };
    enum TextureFilter {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR
    };

    static void hintWrapPattern(TextureWrap s, TextureWrap t);
    static void hintTexFilter(TextureFilter mag, TextureFilter min = Nearest);

    RTexture();
    RTexture(const RImage &img, const std::string &name);
    RTexture(const RTexture &texture);
    RTexture(const RTexture &&texture);
    RTexture& operator=(RTexture texture);
    void swap(RTexture &texture);
    ~RTexture();

    void setWrapPattern(TextureWrap s, TextureWrap t);
    void setTexFilter(TextureFilter mag, TextureFilter min = Nearest);

    bool isValid() const;
    int width() const;
    int height() const;
    RSize size() const;

    bool generate(int width, int height, int echannel, const unsigned char *data, int ichannel = 3);
    bool generate(const RImage &img);
    bool generate(const std::string path);
    void freeTexture();
    void bind(GLenum unit = 0);
    void unbind();

private:
    static void deleteTexture(GLuint *ID);

    static constexpr GLenum TEXTURE_UNIT = GL_TEXTURE0;
    static GLint wrapS;
    static GLint wrapT;
    static GLint filterMin;
    static GLint filterMax;
    static GLuint borderColor[4];

    std::shared_ptr<GLuint> textureID_;
    GLint wrapS_ = wrapS;
    GLint wrapT_ = wrapT;
    GLint filterMin_ = filterMin;
    GLint filterMax_ = filterMax;
    GLuint borderColor_[4]{ borderColor[0], borderColor[1], borderColor[2], borderColor[3] };
    int width_ = 0;
    int height_ = 0;
};

void swap(RTexture &texture1, RTexture &texture2);

#endif // RTEXTURE_H
