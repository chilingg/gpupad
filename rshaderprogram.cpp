#include "rshaderprogram.h"
#include "rdebug.h"

RShaderProgram::RShaderProgram()
{

}

RShaderProgram::RShaderProgram(const RShader &vertex, const RShader &fragment)
{
    attachShader(vertex);
    attachShader(fragment);
    linkProgram();
}

RShaderProgram::RShaderProgram(const RShader &vertex, const RShader &fragment, const RShader &geometry)
{
    attachShader(vertex);
    attachShader(fragment);
    attachShader(geometry);
    linkProgram();
}

bool RShaderProgram::linkProgram()
{
    int  success;
    char infoLog[512];

    glLinkProgram(ID);
    valid = true;
    // 打印编译错误（如果有的话）
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        valid = false;
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        RDebug() << "Eroor: program shader linking failed!";
        exit(EXIT_FAILURE);
    }

    return valid;
}
