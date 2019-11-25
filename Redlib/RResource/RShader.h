#ifndef RSHADER_H
#define RSHADER_H

#include "RResource.h"
#include "ROpenGL.h"

#include <memory>

enum class ShaderType
{
    VertexShader = GL_VERTEX_SHADER,
    FragmentShader = GL_FRAGMENT_SHADER,
    TessContolShader = GL_TESS_CONTROL_SHADER,
    TessEvaluationShader = GL_TESS_EVALUATION_SHADER,
    GeometryShader = GL_GEOMETRY_SHADER,
    ComputeShader = GL_COMPUTE_SHADER
};

class RShader : public RResource
{
public:
    friend void swap(RShader &shader1, RShader &shader2);
    static std::string shaderTypeName(RShader shader);
    static std::string shaderTypeName(ShaderType type);

    explicit RShader();
    RShader(const std::string &path, ShaderType type, const std::string &name);
    RShader(const RShader &shader);
    RShader(const RShader &&shader);
    RShader& operator=(RShader shader);
    void swap(RShader &shader);
    ~RShader();

    bool isValid() const;
    GLuint shaderID() const;
    ShaderType type() const;
    std::string typeName() const;
    bool compileShader(std::string path, ShaderType type);
    bool compileShaderCode(const GLchar *code, ShaderType type);
    void freeShader();

private:
    static void deleteShader(GLuint *shaderID);

    std::shared_ptr<GLuint> shaderID_;
    ShaderType type_;
};

void swap(RShader &shader1, RShader &shader2);

#endif // RSHADER_H
