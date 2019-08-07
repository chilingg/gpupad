#ifndef RSHADERPROGRAM_H
#define RSHADERPROGRAM_H

#include "rresource.h"
#include "rshader.h"

class RShaderProgram : public RResource
{
public:
    RShaderProgram();
    RShaderProgram(const RShader &vertex, const RShader &fragment);
    RShaderProgram(const RShader &vertex, const RShader &fragment, const RShader &geometry);

    void attachShader(const RShader &shader);
    bool linkProgram();
    GLuint getID() const;

protected:
    GLuint ID;
};

inline void RShaderProgram::attachShader(const RShader &shader)
{
    glAttachShader(ID, shader.getShaderID());
}

inline GLuint RShaderProgram::getID() const
{
    return ID;
}

#endif // RSHADERPROGRAM_H
