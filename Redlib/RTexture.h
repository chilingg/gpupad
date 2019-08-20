#ifndef RTEXTURE_H
#define RTEXTURE_H

#include "RResource.h"
#include <glad/glad.h>
#include "RImage.h"

class RTexture : public RResource
{
public:
    RTexture();
    RTexture(const RImage &image);
    ~RTexture();

    void deleteResource();
    bool generate(const RImage &image);
    void bind() const;
    void setParameteri(GLenum pname, GLint param);

    static void unBind();

private:
    GLuint wrapS;
    GLuint wrapT;
    GLuint filterMin;
    GLuint filterMax;
    GLuint ID;
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

inline void RTexture::setParameteri(GLenum pname, GLint param)
{
    glTexParameteri(GL_TEXTURE_2D, pname, param);
}

#endif // RTEXTURE_H
