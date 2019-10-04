#ifndef RTEXTURE_H
#define RTEXTURE_H

#include "RResource.h"
#include <glad/glad.h>
#include "RImage.h"
#include <RDebug.h>

class RTexture : public RResource
{
public:
    RTexture();
    RTexture(const RImage &image);
    RTexture(const RTexture &tex);
    ~RTexture();

    void deleteResource();
    bool generate(const RImage &image);
    bool generate(int width, int height, const unsigned char *data, int channel = GL_RGBA);
    void bind() const;
    GLuint getID() const;

    int width();
    int height();

    static void unBind();

private:
    GLuint _ID;

    GLint wrapS = GL_CLAMP_TO_BORDER;
    GLint wrapT = GL_CLAMP_TO_BORDER;
    GLint filterMin = GL_NEAREST;
    GLint filterMax = GL_NEAREST;
    int _width = 0;
    int _height = 0;
};

inline void RTexture::deleteResource()
{
    if(_ID != INVALID)
    {
        glDeleteTextures(1, &_ID);
        _ID = INVALID;
        state = false;
    }
}

inline void RTexture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, _ID);
}

inline GLuint RTexture::getID() const
{
    return _ID;
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
