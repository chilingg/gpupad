#include "RShader.h"

#include "RDebug.h"

void swap(RShader &shader1, RShader &shader2)
{
    shader1.swap(shader2);
}

std::string RShader::shaderTypeName(const RShader &shader)
{
    return shaderTypeName(shader.type());
}

std::string RShader::shaderTypeName(ShaderType type)
{
    std::string shaderType;
    switch(type) {
    case ShaderType::VertexShader:
        shaderType = "VertexShader";
        break;
    case ShaderType::FragmentShader:
        shaderType = "FragmentShader";
        break;
    case ShaderType::TessContolShader:
        shaderType = "TessContolShader";
        break;
    case ShaderType::TessEvaluationShader:
        shaderType = "TessEvaluationShader";
        break;
    case ShaderType::GeometryShader:
        shaderType = "GeometryShader";
        break;
    case ShaderType::ComputeShader:
        shaderType = "ComputeShader";
        break;
    }
    return shaderType;
}

RShader::RShader():
    RResource("UnnameShader")
{

}

RShader::RShader(const std::string &path, ShaderType type, const std::string &name):
    RResource(name)
{
    compileShader(path, type);
}

RShader::RShader(const RShader &shader):
    RResource(shader),
    shaderID_(shader.shaderID_),
    type_(shader.type_)
{

}

RShader::RShader(const RShader &&shader):
    RResource(std::move(shader)),
    shaderID_(shader.shaderID_),
    type_(shader.type_)
{

}

RShader &RShader::operator=(RShader shader)
{
    swap(shader);
    return *this;
}

void RShader::swap(RShader &shader)
{
    RResource::swap(shader);
    using std::swap;
    swap(shaderID_, shader.shaderID_);
    swap(type_, shader.type_);
}

RShader::~RShader()
{

}

bool RShader::isValid() const
{
    return shaderID_ != nullptr;
}

GLuint RShader::shaderID() const
{
    return *shaderID_;
}

ShaderType RShader::type() const
{
    return type_;
}

std::string RShader::typeName() const
{
    return shaderTypeName(type_);
}

bool RShader::compileShader(std::string path, ShaderType type)
{
    path = checkFilePath(path);
    if(path.empty())
        return false;

    std::string code = getTextFileContent(path);
    if(code.empty())
        return false;

    return compileShaderCode(code.c_str(), type);
}

bool RShader::compileShaderCode(const GLchar *code, ShaderType type)
{
    shaderID_.reset(new GLuint(glCreateShader(static_cast<GLenum>(type))), deleteShader);
    type_ = type;
    glShaderSource(*shaderID_, 1, &code, nullptr);
    glCompileShader(*shaderID_);

    int success;
    glGetShaderiv(*shaderID_, GL_COMPILE_STATUS, &success);
    if(!success)
    {
#ifdef R_DEBUG
        printError(nameID() + ": " + shaderTypeName(type) + "shader compilation failed!\n");
#endif
        shaderID_.reset();
        return false;
    }

    return true;
}

void RShader::deleteShader(GLuint *ID)
{
    glDeleteShader(*ID);
    delete ID;
}
