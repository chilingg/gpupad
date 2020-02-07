#ifndef RSHADERPROGRAM_H
#define RSHADERPROGRAM_H

#include "RShader.h"
#include "../RMath.h"

#include <initializer_list>

namespace Redopera {

class RShaderProgram : public RResource
{
    friend void swap(RShaderProgram &prog1, RShaderProgram&prog2);

public:
    class Interface
    {
        friend RShaderProgram;

    public:
        ~Interface()
        {
            glUseProgram(0);
            current = nullptr;
        }

        void setViewprot(GLuint loc, float left, float right, float bottom, float top, float near = -127.0f, float far = 128.0f);
        void setPerspective(GLuint loc, float left, float right, float bottom, float top, float near, float far);
        void setCameraPos(GLuint loc, float x, float y, float z = 0);

        void setUniform(GLuint loc, GLfloat v1) const;
        void setUniform(GLuint loc, GLfloat v1, GLfloat v2) const;
        void setUniform(GLuint loc, GLfloat v1, GLfloat v2, GLfloat v3) const;
        void setUniform(GLuint loc, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4) const;
        void setUniform(GLuint loc, glm::vec3 vec) const;
        void setUniform(GLuint loc, glm::vec4 vec) const;
        void setUniform(GLuint loc, GLint v1) const;
        void setUniform(GLuint loc, GLint v1, GLint v2) const;
        void setUniform(GLuint loc, GLint v1, GLint v2, GLint v3) const;
        void setUniform(GLuint loc, GLint v1, GLint v2, GLint v3, GLint v4) const;
        void setUniform(GLuint loc, glm::ivec3 vec) const;
        void setUniform(GLuint loc, glm::ivec4 vec) const;
        void setUniform(GLuint loc, GLuint v1) const;
        void setUniform(GLuint loc, GLuint v1, GLuint v2) const;
        void setUniform(GLuint loc, GLuint v1, GLuint v2, GLuint v3) const;
        void setUniform(GLuint loc, GLuint v1, GLuint v2, GLuint v3, GLuint v4) const;
        void setUniform(GLuint loc, glm::uvec3 vec) const;
        void setUniform(GLuint loc, glm::uvec4 vec) const;
        void setUniform(GLuint loc, GLsizei size, GLfloat *vp, GLsizei count) const;
        void setUniform(GLuint loc, GLsizei size, GLint *vp, GLsizei count) const;
        void setUniform(GLuint loc, GLsizei size, GLuint *vp, GLsizei count) const;
        void setUniform(GLuint loc, glm::vec3 *vec, GLsizei count) const;
        void setUniform(GLuint loc, glm::vec4 *vec, GLsizei count) const;
        void setUniform(GLuint loc, glm::ivec3 *vec, GLsizei count) const;
        void setUniform(GLuint loc, glm::ivec4 *vec, GLsizei count) const;
        void setUniform(GLuint loc, glm::uvec3 *vec, GLsizei count) const;
        void setUniform(GLuint loc, glm::uvec4 *vec, GLsizei count) const;
        void setUniformMatrix(GLuint loc, const glm::mat2 &mat, GLsizei count = 1) const;
        void setUniformMatrix(GLuint loc, const glm::mat3 &mat, GLsizei count = 1) const;
        void setUniformMatrix(GLuint loc, const glm::mat4 &mat, GLsizei count = 1) const;
        void setUniformMatrix(GLuint loc, const glm::dmat2 &mat, GLsizei count = 1) const;
        void setUniformMatrix(GLuint loc, const glm::dmat3 &mat, GLsizei count = 1) const;
        void setUniformMatrix(GLuint loc, const glm::dmat4 &mat, GLsizei count = 1) const;
        void setUniformMatrix(GLuint loc, GLsizei order, GLfloat *vp, GLsizei count = 1, GLboolean transpose = false) const;
        void setUniformMatrix(GLuint loc, GLsizei order, GLdouble *vp, GLsizei count = 1, GLboolean transpose = false) const;

    private:
        Interface(GLuint id):
            id_(id)
        {
            if(current) throw std::logic_error("The current thread has other <Interfaces> is using!");
            current = this;
            glUseProgram(id_);
        }

        thread_local static Interface* current;

        GLuint id_;
    };

    RShaderProgram();
    RShaderProgram(std::initializer_list<RShader*> list, const std::string &name = "ShaderProgram");
    RShaderProgram(const RShaderProgram &program);
    RShaderProgram(const RShaderProgram &&program);
    RShaderProgram& operator=(RShaderProgram program);
    ~RShaderProgram() = default;
    void swap(RShaderProgram &program);

    bool isValid() const;
    bool isAttachShader(ShaderType typr) const;
    GLuint shaderProgramID() const;
    Interface useInterface() const;

    GLuint getUniformLocation(const std::string &name);

    // attachShader() 与 detachShader() 都是只在重新linkProgram时生效
    void attachShader(std::initializer_list<RShader> list);
    void detachShader(ShaderType type);
    bool linkProgram();
    void reLinkProgram();
    void releaseShader();
    void release();

private:
    static void deleteShaderProgram(GLuint *ID);

    std::shared_ptr<GLuint> progID_;
    std::map<ShaderType, RShader> shaders_;
};

} // Redopera

void swap(Redopera::RShaderProgram &prog1, Redopera::RShaderProgram&prog2);

#endif // RSHADERPROGRAM_H
