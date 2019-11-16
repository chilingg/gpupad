#include "RShaderProgram.h"

#include "RDebug.h"

RShaderProgram::RShaderProgram():
    RResource()
{

}

RShaderProgram::RShaderProgram(RShader vertex, RShader fragment)
{
#ifdef R_DEBUG
    if(vertex.type() != RShader::VertexShader && fragment.type() != RShader::FragmentShader)
    {
        printError("Instead of " + RShader::shaderTypeName(vertex.type()) + " and "
                   + RShader::shaderTypeName(fragment.type())
                   + "The program needs VertexShader and FragmentShader!");
        return;
    }
#endif
    attachShader(vertex);
    attachShader(fragment);
    linkProgram();
}

bool RShaderProgram::isValid()
{
    return ID_ != nullptr;
}

void RShaderProgram::attachShader(const RShader &shader)
{
#ifdef R_DEBUG
    printError(shaders_.count(shader.type()), "Invalid setting, The "
               + RShader::shaderTypeName(shader.type()) + "already exists!");
    printError(!shader.isValid(), RShader::shaderTypeName(shader) + " is invalid!");
#endif

    shaders_[shader.type()] = shader.ID();
}

bool RShaderProgram::detachShader(RShader::ShaderType type)
{
    return shaders_.erase(type);
}

bool RShaderProgram::linkProgram()
{
#ifdef R_DEBUG
    printError(!shaders_.count(RShader::VertexShader) && !shaders_.count(RShader::FragmentShader),
               "Unattached either VertexShader or FragmentShader");
#endif

    ID_.reset(new GLuint(glCreateProgram()), deleteShaderProgram);
    for(auto shader: shaders_)
    {
        glAttachShader(*ID_, shader.second);
    }

    int  success;
    char infoLog[512];
    glLinkProgram(*ID_);

#ifdef R_DEBUG
    // 打印编译错误（如果有的话）
    glGetProgramiv(*ID_, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(*ID_, 512, nullptr, infoLog);
        RDebug() << "Eroor: program shader linking failed!" << infoLog;
        ID_.reset();
    }
#endif

    return ID_ != nullptr;
}

void RShaderProgram::use()
{
#ifdef R_DEBUG
    printError(!ID_, "Used invalid shader program!");
#endif

    glUseProgram(*ID_);
    return;
}

void RShaderProgram::nonuse()
{
    glUseProgram(0);
}

void RShaderProgram::freeShaderProgram()
{
    ID_.reset();
}

UniformLocation RShaderProgram::getUniformLocation(const std::string &name)
{
#ifdef R_DEBUG
    printError(!ID_, "Unable to get uniform location for " + name + " Program unlink!");
#endif

    UniformLocation loc = glGetUniformLocation(*ID_, name.c_str());

#ifdef R_DEBUG
    printError(!loc.isValid(), "No find uniform location of the " + name);
#endif

    return loc;
}

void RShaderProgram::setUniform(UniformLocation location, GLfloat v1)
{
#ifdef R_DEBUG
    printError(!ID_, "Setting uniform is invalid because the program is not connected!");
#endif
    glUniform1f(location, v1);
}

void RShaderProgram::setUniform(UniformLocation location, GLfloat v1, GLfloat v2)
{
#ifdef R_DEBUG
    printError(!ID_, "Setting uniform is invalid because the program is not connected!");
#endif
    glUniform2f(location, v1, v2);
}

void RShaderProgram::setUniform(UniformLocation location, GLfloat v1, GLfloat v2, GLfloat v3)
{
#ifdef R_DEBUG
    printError(!ID_, "Setting uniform is invalid because the program is not connected!");
#endif
    glUniform3f(location, v1, v2, v3);
}

void RShaderProgram::setUniform(UniformLocation location, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4)
{
#ifdef R_DEBUG
    printError(!ID_, "Setting uniform is invalid because the program is not connected!");
#endif
    glUniform4f(location, v1, v2, v3, v4);
}

void RShaderProgram::setUniform(UniformLocation location, GLint v1)
{
#ifdef R_DEBUG
    printError(!ID_, "Setting uniform is invalid because the program is not connected!");
#endif
    glUniform1i(location, v1);
}

void RShaderProgram::setUniform(UniformLocation location, GLint v1, GLint v2)
{
#ifdef R_DEBUG
    printError(!ID_, "Setting uniform is invalid because the program is not connected!");
#endif
    glUniform2i(location, v1, v2);
}

void RShaderProgram::setUniform(UniformLocation location, GLint v1, GLint v2, GLint v3)
{
#ifdef R_DEBUG
    printError(!ID_, "Setting uniform is invalid because the program is not connected!");
#endif
    glUniform3i(location, v1, v2, v3);
}

void RShaderProgram::setUniform(UniformLocation location, GLint v1, GLint v2, GLint v3, GLint v4)
{
#ifdef R_DEBUG
    printError(!ID_, "Setting uniform is invalid because the program is not connected!");
#endif
    glUniform4i(location, v1, v2, v3, v4);
}

void RShaderProgram::setUniform(UniformLocation location, GLuint v1)
{
#ifdef R_DEBUG
    printError(!ID_, "Setting uniform is invalid because the program is not connected!");
#endif
    glUniform1ui(location, v1);
}

void RShaderProgram::setUniform(UniformLocation location, GLuint v1, GLuint v2)
{
#ifdef R_DEBUG
    printError(!ID_, "Setting uniform is invalid because the program is not connected!");
#endif
    glUniform2ui(location, v1, v2);
}

void RShaderProgram::setUniform(UniformLocation location, GLuint v1, GLuint v2, GLuint v3)
{
#ifdef R_DEBUG
    printError(!ID_, "Setting uniform is invalid because the program is not connected!");
#endif
    glUniform3ui(location, v1, v2, v3);
}

void RShaderProgram::setUniform(UniformLocation location, GLuint v1, GLuint v2, GLuint v3, GLuint v4)
{
#ifdef R_DEBUG
    printError(!ID_, "Setting uniform is invalid because the program is not connected!");
#endif
    glUniform4ui(location, v1, v2, v3, v4);
}

void RShaderProgram::setUniform(UniformLocation location, GLsizei size, GLfloat *vp, GLsizei count)
{
#ifdef R_DEBUG
    printError(!ID_, "Setting uniform is invalid because the program is not connected!");
#endif

    switch(size) {
    case 1:
        glUniform1fv(location, count, vp);
        break;
    case 2:
        glUniform2fv(location, count, vp);
        break;
    case 3:
        glUniform3fv(location, count, vp);
        break;
    case 4:
        glUniform4fv(location, count, vp);
        break;
    default:
        printError("Invalid size argument for Uniform" + std::to_string(size) + "fv");
        break;
    }
}

void RShaderProgram::setUniform(UniformLocation location, GLsizei size, GLint *vp, GLsizei count)
{
#ifdef R_DEBUG
    printError(!ID_, "Setting uniform is invalid because the program is not connected!");
#endif
    switch(size) {
    case 1:
        glUniform1iv(location, count, vp);
        break;
    case 2:
        glUniform2iv(location, count, vp);
        break;
    case 3:
        glUniform3iv(location, count, vp);
        break;
    case 4:
        glUniform4iv(location, count, vp);
        break;
    default:
        printError("Invalid size argument for Uniform" + std::to_string(size) + "iv");
        break;
    }
}

void RShaderProgram::setUniform(UniformLocation location, GLsizei size, GLuint *vp, GLsizei count)
{
#ifdef R_DEBUG
    printError(!ID_, "Setting uniform is invalid because the program is not connected!");
#endif
    switch(size) {
    case 1:
        glUniform1uiv(location, count, vp);
        break;
    case 2:
        glUniform2uiv(location, count, vp);
        break;
    case 3:
        glUniform3uiv(location, count, vp);
        break;
    case 4:
        glUniform4uiv(location, count, vp);
        break;
    default:
        printError("Invalid size argument for Uniform" + std::to_string(size) + "uiv");
        break;
    }
}

void RShaderProgram::setUniformMatrix(UniformLocation location, GLsizei size, GLfloat *vp, GLsizei count, GLboolean transpose)
{
#ifdef R_DEBUG
    printError(!ID_, "Setting uniform is invalid because the program is not connected!");
#endif
    switch(size) {
    case 2:
        glUniformMatrix2fv(location, count, transpose, vp);
        break;
    case 3:
        glUniformMatrix3fv(location, count, transpose, vp);
        break;
    case 4:
        glUniformMatrix4fv(location, count, transpose, vp);
        break;
    default:
        printError("Invalid size argument for UniformMatrix" + std::to_string(size) + "fv");
        break;
    }
}

void RShaderProgram::setUniformMatrix(UniformLocation location, GLsizei size, GLdouble *vp, GLsizei count, GLboolean transpose)
{
#ifdef R_DEBUG
    printError(!ID_, "Setting uniform is invalid because the program is not connected!");
#endif
    switch(size) {
    case 2:
        glUniformMatrix2dv(location, count, transpose, vp);
        break;
    case 3:
        glUniformMatrix3dv(location, count, transpose, vp);
        break;
    case 4:
        glUniformMatrix4dv(location, count, transpose, vp);
        break;
    default:
        printError("Invalid size argument for UniformMatrix" + std::to_string(size) + "fv");
        break;
    }
}

void RShaderProgram::setUniformMatrix(UniformLocation location, GLsizei column, GLsizei row, GLfloat *vp, GLsizei count, GLboolean transpose)
{
#ifdef R_DEBUG
    printError(!ID_, "Setting uniform is invalid because the program is not connected!");
    printError(row < 2 || row > 4 || column < 2 || column > 4 || row == column,
               "Invalid size argument for UniformMatrix" + std::to_string(column) + 'x' + std::to_string(row));
#endif
    switch(column) {
    case 2:
        if(row == 3) glUniformMatrix2x3fv(location, count, transpose, vp);
        if(row == 4) glUniformMatrix2x4fv(location, count, transpose, vp);
        break;
    case 3:
        if(row == 2) glUniformMatrix3x2fv(location, count, transpose, vp);
        if(row == 4) glUniformMatrix3x4fv(location, count, transpose, vp);
        break;
    case 4:
        if(row == 2) glUniformMatrix4x2fv(location, count, transpose, vp);
        if(row == 3) glUniformMatrix4x3fv(location, count, transpose, vp);
        break;
    }
}

void RShaderProgram::setUniformMatrix(UniformLocation location, GLsizei column, GLsizei row, GLdouble *vp, GLsizei count, GLboolean transpose)
{
#ifdef R_DEBUG
    printError(!ID_, "Setting uniform is invalid because the program is not connected!");
    printError(row < 2 || row > 4 || column < 2 || column > 4 || row == column,
               "Invalid size argument for UniformMatrix" + std::to_string(column) + 'x' + std::to_string(row));
#endif
    switch(column) {
    case 2:
        if(row == 3) glUniformMatrix2x3dv(location, count, transpose, vp);
        if(row == 4) glUniformMatrix2x4dv(location, count, transpose, vp);
        break;
    case 3:
        if(row == 2) glUniformMatrix3x2dv(location, count, transpose, vp);
        if(row == 4) glUniformMatrix3x4dv(location, count, transpose, vp);
        break;
    case 4:
        if(row == 2) glUniformMatrix4x2dv(location, count, transpose, vp);
        if(row == 3) glUniformMatrix4x3dv(location, count, transpose, vp);
        break;
    }
}

void RShaderProgram::deleteShaderProgram(GLuint *ID)
{
    glDeleteProgram(*ID);
    delete ID;
}
