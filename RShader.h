#ifndef RSHADER_H
#define RSHADER_H

#include "RResource.h"
#include <glad/glad.h>

class RShader : protected RResource
{
public:
    RShader();
    RShader(const GLchar* shaderPath, GLenum type);
    ~RShader();

    void deleteResource();
    bool compileShader(const GLchar *path, GLenum type);
    GLuint getShaderID() const;

private:
    GLuint shaderID;
};

inline void RShader::deleteResource()
{
    if(state)
    {
        glDeleteShader(shaderID);
        state = false;
    }
}

inline GLuint RShader::getShaderID() const
{
    return shaderID;
}

#endif // RSHADER_H
