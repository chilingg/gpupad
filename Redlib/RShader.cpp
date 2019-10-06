#include "RShader.h"
#include "RDebug.h"

RShader::RShader():
    RResource(),
    shaderID(std::make_shared<GLuint>(0))
{

}

RShader::RShader(const GLchar *shaderPath, GLenum type):
    RShader()
{
    compileShader(shaderPath, type);
}

RShader::RShader(const std::string &shaderPath, GLenum type):
    RShader(shaderPath.c_str(), type)
{
}

RShader &RShader::operator=(const RShader &shader)
{
    RShader temp(*this);
    shaderID = shader.shaderID;
    return *this;
}

RShader::~RShader()
{
    if(shaderID.unique() && *shaderID)
        glDeleteShader(*shaderID);
}

bool RShader::compileShader(const GLchar *path, GLenum type)
{
    auto rePath = checkResourcePath(path);
    if(rePath.empty())
        return false;

    std::string code = openTextFile(path);
    const char* shaderCode = code.c_str();

    return compileShaderCode(shaderCode, type);
}

bool RShader::compileShaderCode(const GLchar *code, GLenum type)
{
    int success;
    char infoLog[512];

    if(!shaderID.unique())
        shaderID = std::make_shared<GLuint>(0);
    else if(*shaderID)
        glDeleteShader(*shaderID);

    //创建编译顶点着色器
    *shaderID = glCreateShader(type);
    glShaderSource(*shaderID, 1, &code, nullptr);
    glCompileShader(*shaderID);
    //若有错误，则打印
    glGetShaderiv(*shaderID, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(*shaderID, 512, nullptr, infoLog);
        std::string shaderType;
        if(type == GL_VERTEX_SHADER)
            shaderType = "Vertex";
        else if(type == GL_FRAGMENT_SHADER)
            shaderType = "Fragment";
        else if(type == GL_GEOMETRY_SHADER)
            shaderType = "Geometry";
        else
            shaderType = "Uknow";

        std::string err = "Eroor: ";
        err += shaderType + "shader compilation failed:\n" + infoLog;
        printErro(err);
        *shaderID = 0;
        exit(EXIT_FAILURE);
    }

    return *shaderID;
}
