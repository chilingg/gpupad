#ifndef RSHADERPROGRAM_H
#define RSHADERPROGRAM_H

#include "RResource.h"
#include "ROpenGL.h"
#include "RShader.h"

#include <memory>
#include <map>

class UniformLocation;

class RShaderProgram : public RResource
{
public:
    RShaderProgram();
    RShaderProgram(RShader vertex, RShader fragment);

    bool isValid();

    void attachShader(const RShader &shader);
    bool detachShader(RShader::ShaderType type);
    bool linkProgram();
    void use();
    void nonuse();
    void freeShaderProgram();
    UniformLocation getUniformLocation(const std::string &name);
    void setUniform(UniformLocation location, GLfloat v1);
    void setUniform(UniformLocation location, GLfloat v1, GLfloat v2);
    void setUniform(UniformLocation location, GLfloat v1, GLfloat v2, GLfloat v3);
    void setUniform(UniformLocation location, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4);
    void setUniform(UniformLocation location, GLint v1);
    void setUniform(UniformLocation location, GLint v1, GLint v2);
    void setUniform(UniformLocation location, GLint v1, GLint v2, GLint v3);
    void setUniform(UniformLocation location, GLint v1, GLint v2, GLint v3, GLint v4);
    void setUniform(UniformLocation location, GLuint v1);
    void setUniform(UniformLocation location, GLuint v1, GLuint v2);
    void setUniform(UniformLocation location, GLuint v1, GLuint v2, GLuint v3);
    void setUniform(UniformLocation location, GLuint v1, GLuint v2, GLuint v3, GLuint v4);
    void setUniform(UniformLocation location, GLsizei size, GLfloat *vp, GLsizei count = 1);
    void setUniform(UniformLocation location, GLsizei size, GLint *vp, GLsizei count = 1);
    void setUniform(UniformLocation location, GLsizei size, GLuint *vp, GLsizei count = 1);
    void setUniformMatrix(UniformLocation location, GLsizei size, GLfloat *vp, GLsizei count = 1, GLboolean transpose = false);
    void setUniformMatrix(UniformLocation location, GLsizei size, GLdouble *vp, GLsizei count = 1, GLboolean transpose = false);
    void setUniformMatrix(UniformLocation location, GLsizei column, GLsizei row, GLfloat *vp, GLsizei count = 1, GLboolean transpose = false);
    void setUniformMatrix(UniformLocation location, GLsizei column, GLsizei row, GLdouble *vp, GLsizei count = 1, GLboolean transpose = false);

private:
    static void deleteShaderProgram(GLuint *ID);

    std::shared_ptr<GLuint> ID_;
    std::map<RShader::ShaderType, GLuint> shaders_;
};

class UniformLocation
{
    friend RShaderProgram;
public:
    operator GLint() const { return location_;}
    bool isValid() const { return location_ != -1;}
private:
    UniformLocation(GLint location = -1): location_(location){}
    GLint location_;
};

#endif // RSHADERPROGRAM_H
