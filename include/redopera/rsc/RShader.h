#ifndef RSHADER_H
#define RSHADER_H

#include "RResource.h"
#include "ROpenGL.h"

namespace Redopera {

enum class ShaderType
{
    VertexShader    = GL_VERTEX_SHADER,
    FragmentShader  = GL_FRAGMENT_SHADER,
    TessContolShader        = GL_TESS_CONTROL_SHADER,
    TessEvaluationShader    = GL_TESS_EVALUATION_SHADER,
    GeometryShader  = GL_GEOMETRY_SHADER,
    ComputeShader   = GL_COMPUTE_SHADER
};

class RShader : public RResource
{
    friend void swap(RShader &shader1, RShader &shader2);

public:
    static const std::string& shaderTypeName(ShaderType type);

    RShader();
    RShader(const std::string &shader, ShaderType type, const std::string &name = "Shader");
    RShader(const RShader &shader);
    RShader(const RShader &&shader);
    RShader& operator=(RShader shader);
    void swap(RShader &shader);
    ~RShader() = default;

    bool isValid() const;
    ShaderType type() const;
    const std::string& typeName() const;
    GLuint shaderID() const;

    bool load(const std::string &shader, ShaderType type);
    void release();

private:
    static void deleteShader(GLuint *id);

    std::shared_ptr<GLuint> shaderID_;
    ShaderType type_;
};

} // Redopera

void swap(Redopera::RShader &shader1, Redopera::RShader &shader2);

#endif // RSHADER_H
