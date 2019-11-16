#ifndef RSHADER_H
#define RSHADER_H

#include "RResource.h"
#include "ROpenGL.h"

#include <memory>

class RShader : public RResource
{
public:
    enum ShaderType {
        VertexShader = GL_VERTEX_SHADER,
        FragmentShader = GL_FRAGMENT_SHADER,
        TessContolShader = GL_TESS_CONTROL_SHADER,
        TessEvaluationShader = GL_TESS_EVALUATION_SHADER,
        GeometryShader = GL_GEOMETRY_SHADER,
        ComputeShader = GL_COMPUTE_SHADER
    };
    static std::string shaderTypeName(RShader shader);
    static std::string shaderTypeName(ShaderType type);

    RShader();
    RShader(const std::string &path, ShaderType type);
    ~RShader();

    bool isValid() const;
    GLuint ID() const;
    ShaderType type() const;
    bool compileShader(std::string path, ShaderType type);
    bool compileShaderCode(const GLchar *code, ShaderType type);
    void freeShader();

private:
    static void deleteShader(GLuint *ID);
    std::shared_ptr<GLuint> ID_;
    ShaderType type_;
};

#endif // RSHADER_H
