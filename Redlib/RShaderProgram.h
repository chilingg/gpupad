#ifndef RSHADERPROGRAM_H
#define RSHADERPROGRAM_H

#include "RResource.h"
#include "RShader.h"
#include "RDebug.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>

class RShaderProgram : public RResource
{
public:
    RShaderProgram();
    RShaderProgram(const RShader &vertex, const RShader &fragment);
    RShaderProgram(const RShader &vertex, const RShader &fragment, const RShader &geometry);
    RShaderProgram& operator=(const RShaderProgram &prg);
    ~RShaderProgram() override;

    bool isValid() const override;

    void attachShader(const RShader &shader);
    bool linkProgram();
    void use();
    void setUniform1F(const std::string &name, float value) const;
    void setUniform1I(const std::string &name, int value) const;
    void setUniform2F(const std::string &name, glm::vec2 value) const;
    void setUniform2F(const std::string &name, float value1, float value2) const;
    void setUniform3F(const std::string &name, float value1, float value2, float value3) const;
    void setUniform3F(const std::string &name, glm::vec3 vec) const;
    void setUniform4F(const std::string &name, float value1, float value2, float value3, float value4) const;
    void setUniform4F(const std::string &name, glm::vec4 vec) const;
    void setUniformMatrix4fv(const char *name, float *ptr) const;
    void setUniformMatrix3fv(const char *name, float *ptr) const;

protected:
    std::shared_ptr<GLuint> ID;
    bool valid = false;
    //三个着色器成员变量
};

inline bool RShaderProgram::isValid() const
{
    return *ID;
}

inline void RShaderProgram::attachShader(const RShader &shader)
{
    if(!ID.unique())
        ID = std::make_shared<GLuint>(0);
    //着色器程序ID在变更时无需删除 只需重新分配
    if(!*ID)
        *ID = glCreateProgram();

    glAttachShader(*ID, shader.getShaderID());
}

inline void RShaderProgram::use()
{
    glUseProgram(*ID);
}

inline void RShaderProgram::setUniform1F(const std::string &name, float value) const
{
    int loc = glGetUniformLocation(*ID, name.c_str());
    if(loc != -1)
        glUniform1f(loc, value);
    else
        RDebug() << "No find uniform location of the " << name.c_str();
}

inline void RShaderProgram::setUniform1I(const std::string &name, int value) const
{
    int loc = glGetUniformLocation(*ID, name.c_str());
    if(loc != -1)
        glUniform1i(loc, value);
    else
        RDebug() << "No find uniform location of the " << name.c_str();
}

inline void RShaderProgram::setUniform2F(const std::string &name, glm::vec2 value) const
{
    setUniform2F(name, value.x, value.y);
}

inline void RShaderProgram::setUniform2F(const std::string &name, float value1, float value2) const
{
    int loc = glGetUniformLocation(*ID, name.c_str());
    if(loc != -1)
        glUniform2f(loc, value1, value2);
    else
        RDebug() << "No find uniform location of the " << name.c_str();
}

inline void RShaderProgram::setUniform3F(const std::string &name, float value1, float value2, float value3) const
{
    int loc = glGetUniformLocation(*ID, name.c_str());
    if(loc != -1)
        glUniform3f(loc, value1, value2, value3);
    else
        RDebug() << "No find uniform location of the " << name.c_str();
}

inline void RShaderProgram::setUniform3F(const std::string &name, glm::vec3 vec) const
{
    setUniform3F(name, vec.x, vec.y, vec.z);
}

inline void RShaderProgram::setUniform4F(const std::string &name, float value1, float value2, float value3, float value4) const
{
    int loc = glGetUniformLocation(*ID, name.c_str());
    if(loc != -1)
        glUniform4f(loc, value1, value2, value3, value4);
    else
        RDebug() << "No find uniform location of the " << name.c_str();
}

inline void RShaderProgram::setUniform4F(const std::string &name, glm::vec4 vec) const
{
    setUniform4F(name, vec.x, vec.y, vec.z, vec.w);
}

inline void RShaderProgram::setUniformMatrix4fv(const char *name, float *ptr) const
{
    int loc = glGetUniformLocation(*ID, name);
    if(loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, ptr);
    else
        RDebug() << "No find uniform location " << name;
}

inline void RShaderProgram::setUniformMatrix3fv(const char *name, float *ptr) const
{
    int loc = glGetUniformLocation(*ID, name);
    if(loc != -1)
        glUniformMatrix3fv(loc, 1, GL_FALSE, ptr);
    else
        RDebug() << "No find uniform location " << name;
}

#endif // RSHADERPROGRAM_H
