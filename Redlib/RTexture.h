#ifndef RTEXTURE_H
#define RTEXTURE_H

#include "RResource.h"
#include <glad/glad.h>
#include <memory>
#include "RImage.h"
#include <RDebug.h>

class RTexture : public RResource
{
public:
    RTexture();
    RTexture(const RImage &image);
    RTexture(const RTexture &tex) = default;
    RTexture& operator=(const RTexture &tex);
    ~RTexture() override;

    bool isValid() const override;

    int width();
    int height();
    GLuint getID() const;

    bool generate(const RImage &image);
    bool generate(int width, int height, const unsigned char *data, int channel = GL_RGBA);
    void bind() const;
    static void unBind();

protected:
    std::shared_ptr<GLuint> _ID;
    GLint wrapS = GL_CLAMP_TO_BORDER;
    GLint wrapT = GL_CLAMP_TO_BORDER;
    GLint filterMin = GL_NEAREST;
    GLint filterMax = GL_NEAREST;
    int _width = 0;
    int _height = 0;
};

inline void RTexture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, *_ID);
}

inline GLuint RTexture::getID() const
{
    return *_ID;
}

inline bool RTexture::isValid() const
{
    return *_ID;
}

inline int RTexture::width()
{
    return _width;
}

inline int RTexture::height()
{
    return _height;
}

inline void RTexture::unBind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

#endif // RTEXTURE_H
