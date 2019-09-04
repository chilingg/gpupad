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
    RTexture(std::string path);
    ~RTexture();

    void deleteResource();
    bool generate(std::string path);
    void bind() const;

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

#endif // RTEXTURE_H
