#ifndef RSHADER_H
#define RSHADER_H

#include "RResource.h"
#include <glad/glad.h>
#include <string>
#include <memory>

class RShader : protected RResource
{
public:
    static const GLenum VERTEX_SHADER = GL_VERTEX_SHADER;
    static const GLenum FRAGMENT_SHADER = GL_FRAGMENT_SHADER;
    static const GLenum GEOMETRY_SHADER = GL_GEOMETRY_SHADER;

    RShader();
    RShader(const GLchar* shaderPath, GLenum type);
    RShader(const std::string &shaderPath, GLenum type);
    RShader& operator=(const RShader &shader);
    ~RShader() override;

    bool isValid() const override;
    bool compileShader(const GLchar *path, GLenum type);
    bool compileShaderCode(const GLchar *code, GLenum type);
    GLuint getShaderID() const;

private:
    std::shared_ptr<GLuint> shaderID;
};

inline bool RShader::isValid() const
{
    return glIsShader(*shaderID);
}

inline GLuint RShader::getShaderID() const
{
    return *shaderID;
}

#endif // RSHADER_H
