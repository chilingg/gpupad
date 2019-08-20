#ifndef RSHADERPROGRAM_H
#define RSHADERPROGRAM_H

#include "RResource.h"
#include "RShader.h"
#include "RDebug.h"
#include <glm/vec3.hpp>

class RShaderProgram : public RResource
{
public:
    RShaderProgram();
    RShaderProgram(const RShader &vertex, const RShader &fragment);
    RShaderProgram(const RShader &vertex, const RShader &fragment, const RShader &geometry);
    ~RShaderProgram();

    void deleteResource();
    void attachShader(const RShader &shader);
    bool linkProgram();
    void use();
    void setUniform1F(const std::string &name, float value) const;
    void setUniform1I(const std::string &name, int value) const;
    void setUniform3F(const std::string &name, float value1, float value2, float value3) const;
    void setUniform3F(const std::string &name, glm::vec3 vec) const;
    void setUniformMatrix4fv(const char *name, float *ptr) const;
    void setUniformMatrix3fv(const char *name, float *ptr) const;

private:
    GLuint ID;
};

inline void RShaderProgram::deleteResource()
{
    if(ID != INVALID)
    {
        glDeleteProgram(ID);
        state = false;
        ID = INVALID;
    }
}

inline void RShaderProgram::attachShader(const RShader &shader)
{
    glAttachShader(ID, shader.getShaderID());
}

inline void RShaderProgram::use()
{
    glUseProgram(ID);
}

inline void RShaderProgram::setUniform1F(const std::string &name, float value) const
{
    int loc = glGetUniformLocation(ID, name.c_str());
    if(loc != -1)
        glUniform1f(loc, value);
    else
        RDebug() << "No find uniform location of the " << name.c_str();
}

inline void RShaderProgram::setUniform1I(const std::string &name, int value) const
{
    int loc = glGetUniformLocation(ID, name.c_str());
    if(loc != -1)
        glUniform1i(loc, value);
    else
        RDebug() << "No find uniform location of the " << name.c_str();
}

inline void RShaderProgram::setUniform3F(const std::string &name, float value1, float value2, float value3) const
{
    int loc = glGetUniformLocation(ID, name.c_str());
    if(loc != -1)
        glUniform3f(loc, value1, value2, value3);
    else
        RDebug() << "No find uniform location of the " << name.c_str();
}

inline void RShaderProgram::setUniform3F(const std::string &name, glm::vec3 vec) const
{
    setUniform3F(name, vec.x, vec.y, vec.z);
}

inline void RShaderProgram::setUniformMatrix4fv(const char *name, float *ptr) const
{
    int loc = glGetUniformLocation(ID, name);
    if(loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, ptr);
    else
        RDebug() << "No find uniform location " << name;
}

inline void RShaderProgram::setUniformMatrix3fv(const char *name, float *ptr) const
{
    int loc = glGetUniformLocation(ID, name);
    if(loc != -1)
        glUniformMatrix3fv(loc, 1, GL_FALSE, ptr);
    else
        RDebug() << "No find uniform location " << name;
}

#endif // RSHADERPROGRAM_H
