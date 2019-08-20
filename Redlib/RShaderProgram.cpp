#include "RShaderProgram.h"
#include "RDebug.h"

RShaderProgram::RShaderProgram()
{
    ID = glCreateProgram();
}

RShaderProgram::RShaderProgram(const RShader &vertex, const RShader &fragment):
    RShaderProgram()
{
    attachShader(vertex);
    attachShader(fragment);
    linkProgram();
}

RShaderProgram::RShaderProgram(const RShader &vertex, const RShader &fragment, const RShader &geometry):
    RShaderProgram()
{
    attachShader(vertex);
    attachShader(fragment);
    attachShader(geometry);
    linkProgram();
}

RShaderProgram::~RShaderProgram()
{
    deleteResource();
}

bool RShaderProgram::linkProgram()
{
    int  success;
    char infoLog[512];

    glLinkProgram(ID);
    state = true;
    // 打印编译错误（如果有的话）
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        state = false;
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        RDebug() << "Eroor: program shader linking failed!" << infoLog;
        exit(EXIT_FAILURE);
    }

    return state;
}
