#ifndef RSHADERPROGRAM_H
#define RSHADERPROGRAM_H

#include "RResource.h"
#include "ROpenGL.h"
#include "RShader.h"

#include <memory>
#include <map>

class RUniformLocation;

class RShaderProgram : public RResource
{
    friend void swap(RShaderProgram &prog1, RShaderProgram&prog2);
public:
    RShaderProgram();
    RShaderProgram(RShader vertex, RShader fragment, const std::string &name);
    RShaderProgram(const RShaderProgram &program);
    RShaderProgram(const RShaderProgram &&program);
    RShaderProgram& operator=(RShaderProgram program);
    void swap(RShaderProgram &prog);

    bool isValid() const;

    void attachShader(const RShader &shader);
    void attachShader(const std::string &path, ShaderType type);
    void attachShaderCode(const GLchar *code, ShaderType type);
    bool linkProgram();
    void use() const;
    void nonuse();
    void freeShaderProgram();

    RUniformLocation getUniformLocation(const std::string &name) const;
    void setUniform(RUniformLocation location, GLfloat v1) const;
    void setUniform(RUniformLocation location, GLfloat v1, GLfloat v2) const;
    void setUniform(RUniformLocation location, GLfloat v1, GLfloat v2, GLfloat v3) const;
    void setUniform(RUniformLocation location, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4) const;
    void setUniform(RUniformLocation location, GLint v1) const;
    void setUniform(RUniformLocation location, GLint v1, GLint v2) const;
    void setUniform(RUniformLocation location, GLint v1, GLint v2, GLint v3) const;
    void setUniform(RUniformLocation location, GLint v1, GLint v2, GLint v3, GLint v4) const;
    void setUniform(RUniformLocation location, GLuint v1) const;
    void setUniform(RUniformLocation location, GLuint v1, GLuint v2) const;
    void setUniform(RUniformLocation location, GLuint v1, GLuint v2, GLuint v3) const;
    void setUniform(RUniformLocation location, GLuint v1, GLuint v2, GLuint v3, GLuint v4) const;
    void setUniform(RUniformLocation location, GLsizei size, GLfloat *vp, GLsizei count = 1) const;
    void setUniform(RUniformLocation location, GLsizei size, GLint *vp, GLsizei count = 1) const;
    void setUniform(RUniformLocation location, GLsizei size, GLuint *vp, GLsizei count = 1) const;
    void setUniformMatrix(RUniformLocation location, GLsizei size, GLfloat *vp, GLsizei count = 1, GLboolean transpose = false) const;
    void setUniformMatrix(RUniformLocation location, GLsizei size, GLdouble *vp, GLsizei count = 1, GLboolean transpose = false) const;
    void setUniformMatrix(RUniformLocation location, GLsizei column, GLsizei row, GLfloat *vp, GLsizei count = 1, GLboolean transpose = false) const;
    void setUniformMatrix(RUniformLocation location, GLsizei column, GLsizei row, GLdouble *vp, GLsizei count = 1, GLboolean transpose = false) const;

private:
    static void deleteShaderProgram(GLuint *ID);

    static GLuint usingProgramID;

    std::shared_ptr<GLuint> progID_;
    std::map<ShaderType, RShader> shaders_;
};

class RUniformLocation
{
    friend RShaderProgram;
public:
    RUniformLocation(): location_(-1), name_("UnknowLoc"){}
    operator GLint() const { return location_; }
    bool isValid() const { return location_ != -1; }
    std::string name() const { return name_; }
private:
    RUniformLocation(GLint location, const std::string name):
        location_(location), name_(name) {}
    GLint location_;
    std::string name_;
};

void swap(RShaderProgram &prog1, RShaderProgram&prog2);

#endif // RSHADERPROGRAM_H
