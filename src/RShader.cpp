#include "rsc/RShader.h"
#include "RDebug.h"

using namespace Redopera;

const std::string &RShader::shaderTypeName(ShaderType type)
{
    static const std::string vetex      = "VertexShader";
    static const std::string fragment   = "FragmentShader";
    static const std::string tessc      = "TessContolShader";
    static const std::string tesse      = "TessEvaluationShader";
    static const std::string geometry   = "GeometryShader";
    static const std::string compute    = "ComputeShader";

    switch(type) {
    case ShaderType::VertexShader:
        return vetex;
    case ShaderType::FragmentShader:
        return fragment;
    case ShaderType::TessContolShader:
        return tessc;
    case ShaderType::TessEvaluationShader:
        return tesse;
    case ShaderType::GeometryShader:
        return geometry;
    case ShaderType::ComputeShader:
        return compute;
    }

    return compute;
}

RShader::RShader():
    RResource("Shader", typeid(this).name())
{

}

RShader::RShader(const std::string &shader, ShaderType type, const std::string &name):
    RResource(name, typeid(this).name())
{
    load(shader, type);
}

RShader::RShader(const RShader &shader):
    RResource(shader),
    shaderID_(shader.shaderID_),
    type_(shader.type_)
{

}

RShader::RShader(const RShader &&shader):
    RResource(std::move(shader)),
    shaderID_(std::move(shader.shaderID_)),
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
    shaderID_.swap(shader.shaderID_);
    using std::swap;
    swap(type_, shader.type_);
}

bool RShader::isValid() const
{
    return shaderID_ != nullptr;
}

ShaderType RShader::type() const
{
    return type_;
}

const std::string &RShader::typeName() const
{
    return shaderTypeName(type_);
}

GLuint RShader::shaderID() const
{
    return *shaderID_;
}

bool RShader::load(const std::string &shader, ShaderType type)
{
    std::string code = getTextFileContent(rscpath(shader));
    if(code.empty())
        code = shader;

    std::shared_ptr<GLuint> id(new GLuint(glCreateShader(static_cast<GLenum>(type))), deleteShader);
    const GLchar *cp = code.c_str();
    glShaderSource(*id, 1, &cp, nullptr);
    glCompileShader(*id);

    int success;
    glGetShaderiv(*id, GL_COMPILE_STATUS, &success);
    if(check(!success, "Failed to load " + shaderTypeName(type) + '<' + name() + "> in:\n" + shader))
        return false;

    type_ = type;
    shaderID_.swap(id);
    return true;
}

void RShader::release()
{
    shaderID_.reset();
}

void RShader::deleteShader(GLuint *id)
{
    glDeleteShader(*id);
    delete id;
}

void swap(RShader &shader1, RShader &shader2)
{
    shader1.swap(shader2);
}
