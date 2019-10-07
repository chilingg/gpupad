#include "RShaderProgram.h"
#include "RDebug.h"

RShaderProgram::RShaderProgram():
    RResource(),
    ID(std::make_shared<GLuint>(0))
{
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

RShaderProgram &RShaderProgram::operator=(const RShaderProgram &prg)
{
    RShaderProgram temp(*this);
    ID = prg.ID;
    return *this;
}

RShaderProgram::~RShaderProgram()
{
    if(ID.unique() && *ID)
        glDeleteProgram(*ID);
}

bool RShaderProgram::linkProgram()
{
    int  success;
    char infoLog[512];

    glLinkProgram(*ID);
    // 打印编译错误（如果有的话）
    glGetProgramiv(*ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(*ID, 512, nullptr, infoLog);
        RDebug() << "Eroor: program shader linking failed!" << infoLog;
        glDeleteProgram(*ID);
        *ID = 0;
        exit(EXIT_FAILURE);
    }

    return *ID;
}
