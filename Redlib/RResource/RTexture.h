#ifndef RTEXTURE_H
#define RTEXTURE_H

#include "RResource.h"
#include "RImage.h"
#include "ROpenGL.h"

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

    explicit RTexture();
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

    bool generate(int width, int height, int echannel, const unsigned char *data, int ichannel = 3);
    bool generate(const RImage &img);
    void freeTexture();
    void bind(GLenum unit = 0);
    void unbind();

private:
    static void deleteTexture(GLuint *ID);
    static constexpr GLenum TEXTURE_UNIT = GL_TEXTURE0;

    std::shared_ptr<GLuint> textureID_;
    GLint wrapS_ = GL_CLAMP_TO_BORDER;
    GLint wrapT_ = GL_CLAMP_TO_BORDER;
    GLint filterMin_ = GL_NEAREST;
    GLint filterMax_ = GL_LINEAR;
    GLuint borderColor[4]{ 0, 0, 0, 255 };
    int width_ = 0;
    int height_ = 0;
};

void swap(RTexture &texture1, RTexture &texture2);

#endif // RTEXTURE_H
