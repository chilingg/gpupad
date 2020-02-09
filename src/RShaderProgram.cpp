#include "rsc/RShaderProgram.h"
#include "RDebug.h"

using namespace Redopera;

thread_local GLuint RShaderProgram::Interface::current = 0;
thread_local int RShaderProgram::Interface::count = 0;

RShaderProgram::Interface::~Interface()
{
    --count;
    if(!count)
    {
        glUseProgram(0);
        current = 0;
    }
}

void RShaderProgram::Interface::setViewprot(GLuint loc, float left, float right, float bottom, float top, float near, float far) const
{
    glm::mat4 projection = glm::ortho(left, right, bottom, top, near, far);
    setUniformMatrix(loc, 4, glm::value_ptr(projection));
}

void RShaderProgram::Interface::setPerspective(GLuint loc, float left, float right, float bottom, float top, float near, float far) const
{
    glm::mat4 projection = perspective(left, right, bottom, top, near, far);
    setUniformMatrix(loc, 4, glm::value_ptr(projection));
}

void RShaderProgram::Interface::setCameraMove(GLuint loc, float x, float y, float z) const
{
    glm::mat4 view = glm::translate(glm::mat4(1), {-x, -y, -z});
    setUniformMatrix(loc, 4, glm::value_ptr(view));
}

void RShaderProgram::Interface::setUniform(GLuint loc, GLfloat v1) const
{
    glUniform1f(loc, v1);
}

void RShaderProgram::Interface::setUniform(GLuint loc, GLfloat v1, GLfloat v2) const
{
    glUniform2f(loc, v1, v2);
}

void RShaderProgram::Interface::setUniform(GLuint loc, GLfloat v1, GLfloat v2, GLfloat v3) const
{
    glUniform3f(loc, v1, v2, v3);
}

void RShaderProgram::Interface::setUniform(GLuint loc, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4) const
{
    glUniform4f(loc, v1, v2, v3, v4);
}

void RShaderProgram::Interface::setUniform(GLuint loc, glm::vec3 vec) const
{
    glUniform3f(loc, vec.x, vec.y, vec.z);
}

void RShaderProgram::Interface::setUniform(GLuint loc, glm::vec4 vec) const
{
    glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
}

void RShaderProgram::Interface::setUniform(GLuint loc, GLint v1) const
{
    glUniform1i(loc, v1);
}

void RShaderProgram::Interface::setUniform(GLuint loc, GLint v1, GLint v2) const
{
    glUniform2i(loc, v1, v2);
}

void RShaderProgram::Interface::setUniform(GLuint loc, GLint v1, GLint v2, GLint v3) const
{
    glUniform3i(loc, v1, v2, v3);
}

void RShaderProgram::Interface::setUniform(GLuint loc, GLint v1, GLint v2, GLint v3, GLint v4) const
{
    glUniform4i(loc, v1, v2, v3, v4);
}

void RShaderProgram::Interface::setUniform(GLuint loc, glm::ivec3 vec) const
{
    glUniform3i(loc, vec.x, vec.y, vec.z);
}

void RShaderProgram::Interface::setUniform(GLuint loc, glm::ivec4 vec) const
{
    glUniform4i(loc, vec.x, vec.y, vec.z, vec.w);
}

void RShaderProgram::Interface::setUniform(GLuint loc, GLuint v1) const
{
    glUniform1ui(loc, v1);
}

void RShaderProgram::Interface::setUniform(GLuint loc, GLuint v1, GLuint v2) const
{
    glUniform2ui(loc, v1, v2);
}

void RShaderProgram::Interface::setUniform(GLuint loc, GLuint v1, GLuint v2, GLuint v3) const
{
    glUniform3ui(loc, v1, v2, v3);
}

void RShaderProgram::Interface::setUniform(GLuint loc, GLuint v1, GLuint v2, GLuint v3, GLuint v4) const
{
    glUniform4ui(loc, v1, v2, v3, v4);
}

void RShaderProgram::Interface::setUniform(GLuint loc, glm::uvec3 vec) const
{
    glUniform3ui(loc, vec.x, vec.y, vec.z);
}

void RShaderProgram::Interface::setUniform(GLuint loc, glm::uvec4 vec) const
{
    glUniform4ui(loc, vec.x, vec.y, vec.z, vec.w);
}

void RShaderProgram::Interface::setUniform(GLuint loc, GLsizei size, GLfloat *vp, GLsizei count) const
{
    switch(size) {
    case 1:
        glUniform1fv(loc, count, vp);
        break;
    case 2:
        glUniform2fv(loc, count, vp);
        break;
    case 3:
        glUniform3fv(loc, count, vp);
        break;
    case 4:
        glUniform4fv(loc, count, vp);
        break;
    default:
        throw std::invalid_argument("Set Invalid size argument for Uniform" + std::to_string(size) + "fv");
    }
}

void RShaderProgram::Interface::setUniform(GLuint loc, GLsizei size, GLint *vp, GLsizei count) const
{
    switch(size) {
    case 1:
        glUniform1iv(loc, count, vp);
        break;
    case 2:
        glUniform2iv(loc, count, vp);
        break;
    case 3:
        glUniform3iv(loc, count, vp);
        break;
    case 4:
        glUniform4iv(loc, count, vp);
        break;
    default:
        throw std::invalid_argument("Invalid size argument for Uniform" + std::to_string(size) + "iv");
        break;
    }
}

void RShaderProgram::Interface::setUniform(GLuint loc, GLsizei size, GLuint *vp, GLsizei count) const
{
    switch(size) {
    case 1:
        glUniform1uiv(loc, count, vp);
        break;
    case 2:
        glUniform2uiv(loc, count, vp);
        break;
    case 3:
        glUniform3uiv(loc, count, vp);
        break;
    case 4:
        glUniform4uiv(loc, count, vp);
        break;
    default:
        throw std::invalid_argument("Invalid size argument for Uniform" + std::to_string(size) + "uiv");
        break;
    }
}

void RShaderProgram::Interface::setUniform(GLuint loc, glm::vec3 *vec, GLsizei count) const
{
    glUniform3fv(loc, count, &vec->x);
}

void RShaderProgram::Interface::setUniform(GLuint loc, glm::vec4 *vec, GLsizei count) const
{
    glUniform4fv(loc, count, &vec->x);
}

void RShaderProgram::Interface::setUniform(GLuint loc, glm::ivec3 *vec, GLsizei count) const
{
    glUniform3iv(loc, count, &vec->x);
}

void RShaderProgram::Interface::setUniform(GLuint loc, glm::ivec4 *vec, GLsizei count) const
{
    glUniform4iv(loc, count, &vec->x);
}

void RShaderProgram::Interface::setUniform(GLuint loc, glm::uvec3 *vec, GLsizei count) const
{
    glUniform3uiv(loc, count, &vec->x);
}

void RShaderProgram::Interface::setUniform(GLuint loc, glm::uvec4 *vec, GLsizei count) const
{
    glUniform4uiv(loc, count, &vec->x);
}

void RShaderProgram::Interface::setUniformMatrix(GLuint loc, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(loc, count, GL_FALSE, glm::value_ptr(mat));
}

void RShaderProgram::Interface::setUniformMatrix(GLuint loc, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(loc, count, GL_FALSE, glm::value_ptr(mat));
}

void RShaderProgram::Interface::setUniformMatrix(GLuint loc, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(loc, count, GL_FALSE, glm::value_ptr(mat));
}

void RShaderProgram::Interface::setUniformMatrix(GLuint loc, const glm::dmat2 &mat) const
{
    glUniformMatrix2dv(loc, count, GL_FALSE, glm::value_ptr(mat));
}

void RShaderProgram::Interface::setUniformMatrix(GLuint loc, const glm::dmat3 &mat) const
{
    glUniformMatrix3dv(loc, count, GL_FALSE, glm::value_ptr(mat));
}

void RShaderProgram::Interface::setUniformMatrix(GLuint loc, const glm::dmat4 &mat) const
{
    glUniformMatrix4dv(loc, count, GL_FALSE, glm::value_ptr(mat));
}

void RShaderProgram::Interface::setUniformMatrix(GLuint loc, const glm::mat2 *mat, GLsizei count) const
{
    glUniformMatrix2fv(loc, count, GL_FALSE, glm::value_ptr(*mat));
}

void RShaderProgram::Interface::setUniformMatrix(GLuint loc, const glm::mat3 *mat, GLsizei count) const
{
    glUniformMatrix3fv(loc, count, GL_FALSE, glm::value_ptr(*mat));
}

void RShaderProgram::Interface::setUniformMatrix(GLuint loc, const glm::mat4 *mat, GLsizei count) const
{
    glUniformMatrix4fv(loc, count, GL_FALSE, glm::value_ptr(*mat));
}

void RShaderProgram::Interface::setUniformMatrix(GLuint loc, const glm::dmat2 *mat, GLsizei count) const
{
    glUniformMatrix2dv(loc, count, GL_FALSE, glm::value_ptr(*mat));
}

void RShaderProgram::Interface::setUniformMatrix(GLuint loc, const glm::dmat3 *mat, GLsizei count) const
{
    glUniformMatrix3dv(loc, count, GL_FALSE, glm::value_ptr(*mat));
}

void RShaderProgram::Interface::setUniformMatrix(GLuint loc, const glm::dmat4 *mat, GLsizei count) const
{
    glUniformMatrix4dv(loc, count, GL_FALSE, glm::value_ptr(*mat));
}

void RShaderProgram::Interface::setUniformMatrix(GLuint loc, GLsizei order, GLfloat *vp, GLsizei count, GLboolean transpose) const
{
    switch(order) {
    case 2:
        glUniformMatrix2fv(loc, count, transpose, vp);
        break;
    case 3:
        glUniformMatrix3fv(loc, count, transpose, vp);
        break;
    case 4:
        glUniformMatrix4fv(loc, count, transpose, vp);
        break;
    default:
        throw std::invalid_argument("Invalid size argument for UniformMatrix" + std::to_string(order) + "fv");
        break;
    }
}

void RShaderProgram::Interface::setUniformMatrix(GLuint loc, GLsizei order, GLdouble *vp, GLsizei count, GLboolean transpose) const
{
    switch(order) {
    case 2:
        glUniformMatrix2dv(loc, count, transpose, vp);
        break;
    case 3:
        glUniformMatrix3dv(loc, count, transpose, vp);
        break;
    case 4:
        glUniformMatrix4dv(loc, count, transpose, vp);
        break;
    default:
        throw std::invalid_argument("Invalid size argument for UniformMatrix" + std::to_string(order) + "dv");
        break;
    }
}

RShaderProgram::Interface::Interface(GLuint id)
{
    if(current && current != id) throw std::runtime_error("The current thread has other <Interfaces> is using!");
    current = id;
    ++count;
    glUseProgram(id);
}

RShaderProgram::RShaderProgram():
    RResource("ShaderProgram", typeid(this).name())
{

}

RShaderProgram::RShaderProgram(std::initializer_list<RShader> list, const std::string &name):
    RResource(name, typeid(this).name())
{
    for(auto &shader : list)
        shaders_.emplace(shader.type(), shader);
    linkProgram();
}

RShaderProgram::RShaderProgram(const RShaderProgram &program):
    RResource(program),
    progID_(program.progID_),
    shaders_(program.shaders_)
{

}

RShaderProgram::RShaderProgram(const RShaderProgram &&program):
    RResource(std::move(program)),
    progID_(std::move(program.progID_)),
    shaders_(std::move(program.shaders_))
{

}

RShaderProgram &RShaderProgram::operator=(RShaderProgram program)
{
    swap(program);
    return *this;
}

void RShaderProgram::swap(RShaderProgram &program)
{
    RResource::swap(program);
    progID_.swap(program.progID_);
    shaders_.swap(program.shaders_);
}

bool RShaderProgram::isValid() const
{
    return progID_ != nullptr;
}

bool RShaderProgram::isAttachShader(RShader::Type type) const
{
    return shaders_.count(type);
}

GLuint RShaderProgram::shaderProgramID() const
{
    return *progID_;
}

RShaderProgram::Interface RShaderProgram::useInterface() const
{
    return Interface(*progID_);
}

GLuint RShaderProgram::getUniformLocation(const std::string &name) const
{
    return glGetUniformLocation(*progID_, name.c_str());
}

void RShaderProgram::attachShader(std::initializer_list<RShader> list)
{
    for(auto &shader : list)
        shaders_.emplace(shader.type(), shader);
}

void RShaderProgram::detachShader(RShader::Type type)
{
    shaders_.erase(type);
}

bool RShaderProgram::linkProgram()
{
    std::shared_ptr<GLuint> id(std::shared_ptr<GLuint>(new GLuint(glCreateProgram()), deleteShaderProgram));

    for(auto shader: shaders_)
        glAttachShader(*id, shader.second.shaderID());
    glLinkProgram(*id);

    int success;
    glGetProgramiv(*id, GL_LINK_STATUS, &success);
    if(check(!success, "Failed to link shader program <" + name() + '>'))
    {
        char infoLog[256];
        glGetProgramInfoLog(*id, sizeof(infoLog), nullptr, infoLog);
        prError(infoLog);
        return false;
    }

    progID_.swap(id);
    return true;
}

void RShaderProgram::reLinkProgram()
{
    glLinkProgram(*progID_);
}

void RShaderProgram::releaseShader()
{
    std::map<RShader::Type, RShader> temp;
    shaders_.swap(temp);
}

void RShaderProgram::release()
{
    releaseShader();
    progID_.reset();
}

void RShaderProgram::deleteShaderProgram(GLuint *id)
{
    glDeleteProgram(*id);
    delete id;
}

void swap(RShaderProgram &prog1, RShaderProgram &prog2)
{
    prog1.swap(prog2);
}

void* Redopera::bufOff(size_t off)
{
    return reinterpret_cast<void*>(off);
}

glm::mat4 Redopera::perspective(float left, float right, float bottom, float top, float near, float far)
{
    static float accuracy = 0.95; // 投影之后far平面拥有的最大z轴标量

    glm::mat4 mat(1);
    float len = far - near;

    // 缩放
    mat[0][0] = 1 / (right - left) * 2;
    mat[1][1] = 1 / (top - bottom) * 2;
    // z轴翻转与投影
    mat[2][3] = -1 / ((2*accuracy*len - 2*len) / (1+accuracy));
    mat[2][2] = mat[2][3];
    // 左乘以平移矩阵
    mat[3][0] = mat[0][0] * -(left + (right - left) / 2);
    mat[3][1] = mat[1][1] * -(bottom + (top - bottom) / 2);
    mat[3][2] = mat[2][2] * -near;
    mat[3][3] = mat[3][2] + 1;

    return mat;
}
