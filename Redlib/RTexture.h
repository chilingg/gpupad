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
    ~RTexture();

    void deleteResource();
    bool generate(const RImage &image);
    void bind() const;

    int width();
    int height();

    static void unBind();

private:
    GLint wrapS;
    GLint wrapT;
    GLint filterMin;
    GLint filterMax;
    GLuint ID;

    int _width = 0;
    int _height = 0;
};

inline void RTexture::deleteResource()
{
    if(ID != INVALID)
    {
        glDeleteTextures(1, &ID);
        ID = INVALID;
        state = false;
    }
}

inline void RTexture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, ID);
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
