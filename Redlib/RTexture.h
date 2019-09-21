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
    RTexture(std::string imagePath);
    ~RTexture();

    void deleteResource();
    bool generate(std::string imagePath);
    void bind() const;

    static void unBind();

private:
    GLint wrapS;
    GLint wrapT;
    GLint filterMin;
    GLint filterMax;
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

inline void RTexture::unBind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

#endif // RTEXTURE_H
