#include "RShader.h"

#include "RDebug.h"

std::string RShader::shaderTypeName(RShader shader)
{
    return shaderTypeName(shader.type());
}

std::string RShader::shaderTypeName(RShader::ShaderType type)
{
    std::string shaderType;
    switch(type) {
    case VertexShader:
        shaderType = "VertexShader";
        break;
    case FragmentShader:
        shaderType = "FragmentShader";
        break;
    case TessContolShader:
        shaderType = "TessContolShader";
        break;
    case TessEvaluationShader:
        shaderType = "TessEvaluationShader";
        break;
    case GeometryShader:
        shaderType = "GeometryShader";
        break;
    case ComputeShader:
        shaderType = "ComputeShader";
        break;
    }
    return shaderType;
}

RShader::RShader():
    RResource()
{

}

RShader::RShader(const std::string &path, ShaderType type)
{
    compileShader(path, type);
}

RShader::~RShader()
{

}

bool RShader::isValid() const
{
    return ID_ != nullptr;
}

GLuint RShader::ID() const
{
    return *ID_;
}

RShader::ShaderType RShader::type() const
{
    return type_;
}

bool RShader::compileShader(std::string path, ShaderType type)
{
    path = checkFilePath(path);
    if(path.empty())
        return false;

    std::string code = getTextFileContent(path);

    return compileShaderCode(code.c_str(), type);
}

bool RShader::compileShaderCode(const GLchar *code, ShaderType type)
{
    ID_.reset(new GLuint(glCreateShader(type)), deleteShader);
    type_ = type;
    glShaderSource(*ID_, 1, &code, nullptr);
    glCompileShader(*ID_);

    int success;
    char infoLog[512];
    //若有错误，则打印
    glGetShaderiv(*ID_, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(*ID_, 512, nullptr, infoLog);

        printError("Eroor: " + shaderTypeName(type) + "shader compilation failed:\n" + infoLog);
        ID_.reset();
    }

    return ID_ != nullptr;
}

void RShader::deleteShader(GLuint *ID)
{
    glDeleteShader(*ID);
    delete ID;
}
