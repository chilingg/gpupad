#ifndef RSHADER_H
#define RSHADER_H

#include "RResource.h"
#include <glad/glad.h>
#include <string>

class RShader : protected RResource
{
public:
    static const GLenum VERTEX_SHADER = GL_VERTEX_SHADER;
    static const GLenum FRAGMENT_SHADER = GL_FRAGMENT_SHADER;
    static const GLenum GEOMETRY_SHADER = GL_GEOMETRY_SHADER;

    RShader();
    RShader(const GLchar* shaderPath, GLenum type);
    RShader(const std::string &shaderPath, GLenum type);
    ~RShader();

    void deleteResource();
    bool compileShader(const GLchar *path, GLenum type);
    bool compileShaderCode(const GLchar *code, GLenum type);
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
