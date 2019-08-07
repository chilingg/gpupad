#ifndef RSHADER_H
#define RSHADER_H

#include "rresource.h"
#include <glad/glad.h>

class RShader : protected RResource
{
public:
    RShader();
    RShader(const GLchar* shaderPath, GLenum type);

    bool compileShader(const GLchar *path, GLenum type);
    void deleteShader();
    GLuint getShaderID() const;

protected:
    GLuint shaderID;
};

inline void RShader::deleteShader()
{
    valid = false;
    glDeleteShader(shaderID);
}

inline GLuint RShader::getShaderID() const
{
    return shaderID;
}

#endif // RSHADER_H
