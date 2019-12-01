#include "RShaderProgram.h"

#include "RDebug.h"

GLuint RShaderProgram::usingProgramID(0);

void swap(RShaderProgram &prog1, RShaderProgram &prog2)
{
    prog1.swap(prog2);
}

RShaderProgram::RShaderProgram():
    RResource("UnknowShaderProgram")
{

}

RShaderProgram::RShaderProgram(RShader vertex, RShader fragment, const std::string &name):
    RResource(name)
{
#ifdef R_DEBUG
    if(vertex.type() != ShaderType::VertexShader && fragment.type() != ShaderType::FragmentShader)
    {
        printError("Instead of " + vertex.name() + ' ' + vertex.typeName()
                   + " and " + fragment.name() + ' ' + fragment.typeName()
                   + " The program needs VertexShader and FragmentShader!");
        return;
    }
#endif
    attachShader(vertex);
    attachShader(fragment);
    linkProgram();
}

RShaderProgram::RShaderProgram(const RShaderProgram &program):
    RResource(program),
    progID_(program.progID_),
    shaders_(program.shaders_)
{

}

RShaderProgram::RShaderProgram(const RShaderProgram &&program):
    RResource(program),
    progID_(program.progID_),
    shaders_(program.shaders_)
{

}

RShaderProgram &RShaderProgram::operator=(RShaderProgram program)
{
    swap(program);
    return *this;
}

void RShaderProgram::swap(RShaderProgram &prog)
{
    RResource::swap(prog);
    using std::swap;
    swap(progID_, prog.progID_);
    swap(shaders_, prog.shaders_);
}

bool RShaderProgram::isValid() const
{
    return progID_ != nullptr;
}

void RShaderProgram::attachShader(const RShader &shader)
{
#ifdef R_DEBUG
    if(printError(shaders_.count(shader.type()), "Invalid setting, The " + shader.name() + ' '
               + shader.typeName() + "already exists!"))
        return;
    if(printError(!shader.isValid(), shader.nameID() + " is invalid!"))
        return;
#endif

    shaders_[shader.type()] = shader;
}

void RShaderProgram::attachShader(const std::string &path, ShaderType type)
{
#ifdef R_DEBUG
    if(printError(shaders_.count(type), "Invalid setting, The " + RShader::shaderTypeName(type)
                  + ' ' + "already exists!"))
        return;
#endif
    RShader shader;
    shader.compileShader(path, type);
#ifdef R_DEBUG
    if(printError(!shader.isValid(), shader.nameID() + shader.typeName() + " is invalid!"))
        return;
#endif

    shaders_[shader.type()] = shader;
}

void RShaderProgram::attachShaderCode(const GLchar *code, ShaderType type)
{
#ifdef R_DEBUG
    if(printError(shaders_.count(type), "Invalid setting, The " + RShader::shaderTypeName(type)
                  + ' ' + "already exists!"))
        return;
#endif
    RShader shader;
    shader.compileShaderCode(code, type);
#ifdef R_DEBUG
    if(printError(!shader.isValid(), shader.nameID() + shader.typeName() + " is invalid!"))
        return;
#endif

    shaders_[shader.type()] = shader;
}

bool RShaderProgram::linkProgram()
{
#ifdef R_DEBUG
    if(printError(!shaders_.count(ShaderType::VertexShader) && !shaders_.count(ShaderType::FragmentShader),
               "Unattached either VertexShader or FragmentShader"))
        return false;
#endif

    progID_.reset(new GLuint(glCreateProgram()), deleteShaderProgram);
    for(auto shader: shaders_)
    {
        glAttachShader(*progID_, shader.second.shaderID());
    }

    int  success;
    glLinkProgram(*progID_);

#ifdef R_DEBUG
    // 打印编译错误（如果有的话）
    glGetProgramiv(*progID_, GL_LINK_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(*progID_, 512, nullptr, infoLog);
        RDebug() << infoLog;
        printError(nameID() + ": Shader program linking failed!");
        progID_.reset();
        return false;
    }
#endif

    std::map<ShaderType, RShader> temp;
    shaders_.swap(temp);//连接后清空保存的着色器
    return true;
}

void RShaderProgram::use() const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Not be used " + nameID() + "Its invalid shader program!"))
        return;
#endif

    usingProgramID = *progID_;
    glUseProgram(usingProgramID);
    return;
}

void RShaderProgram::nonuse()
{
    usingProgramID = 0;
    glUseProgram(0);
}

void RShaderProgram::freeShaderProgram()
{
    shaders_.clear();
    progID_.reset();
}

RUniformLocation RShaderProgram::getUniformLocation(const std::string &name) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Unable to get uniform location " + name + " for " + nameID()
               + ", Its invalid shader program!")) return RUniformLocation();
    if(printError(usingProgramID != *progID_, "Getting uniform " + name
                  + " is invalid for " + nameID() + ", Its not have use!")) return RUniformLocation();
#endif

    RUniformLocation loc(glGetUniformLocation(*progID_, name.c_str()), name);

#ifdef R_DEBUG
    if(printError(!loc.isValid(), "No find uniform location of the " + name + " for " + nameID()))
        loc = RUniformLocation();
#endif

    return loc;
}

void RShaderProgram::setUniform(RUniformLocation location, GLfloat v1) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid location!")) return;
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid shader program!")) return;
    if(printError(usingProgramID != *progID_, "Setting uniform " + location.name_
                  + " is invalid for " + nameID() + ", Its not have use!")) return;
#endif
    glUniform1f(location, v1);
}

void RShaderProgram::setUniform(RUniformLocation location, GLfloat v1, GLfloat v2) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid location!")) return;
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid shader program!")) return;
    if(printError(usingProgramID != *progID_, "Setting uniform " + location.name_
                  + " is invalid for " + nameID() + ", Its not have use!")) return;
#endif
    glUniform2f(location, v1, v2);
}

void RShaderProgram::setUniform(RUniformLocation location, GLfloat v1, GLfloat v2, GLfloat v3) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid location!")) return;
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid shader program!")) return;
    if(printError(usingProgramID != *progID_, "Setting uniform " + location.name_
                  + " is invalid for " + nameID() + ", Its not have use!")) return;
#endif
    glUniform3f(location, v1, v2, v3);
}

void RShaderProgram::setUniform(RUniformLocation location, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid location!")) return;
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid shader program!")) return;
    if(printError(usingProgramID != *progID_, "Setting uniform " + location.name_
                  + " is invalid for " + nameID() + ", Its not have use!")) return;
#endif
    glUniform4f(location, v1, v2, v3, v4);
}

void RShaderProgram::setUniform(RUniformLocation location, GLint v1) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid location!")) return;
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid shader program!")) return;
    if(printError(usingProgramID != *progID_, "Setting uniform " + location.name_
                  + " is invalid for " + nameID() + ", Its not have use!")) return;
#endif
    glUniform1i(location, v1);
}

void RShaderProgram::setUniform(RUniformLocation location, GLint v1, GLint v2) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid location!")) return;
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid shader program!")) return;
    if(printError(usingProgramID != *progID_, "Setting uniform " + location.name_
                  + " is invalid for " + nameID() + ", Its not have use!")) return;
#endif
    glUniform2i(location, v1, v2);
}

void RShaderProgram::setUniform(RUniformLocation location, GLint v1, GLint v2, GLint v3) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid location!")) return;
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid shader program!")) return;
    if(printError(usingProgramID != *progID_, "Setting uniform " + location.name_
                  + " is invalid for " + nameID() + ", Its not have use!")) return;
#endif
    glUniform3i(location, v1, v2, v3);
}

void RShaderProgram::setUniform(RUniformLocation location, GLint v1, GLint v2, GLint v3, GLint v4) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid location!")) return;
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid shader program!")) return;
    if(printError(usingProgramID != *progID_, "Setting uniform " + location.name_
                  + " is invalid for " + nameID() + ", Its not have use!")) return;
#endif
    glUniform4i(location, v1, v2, v3, v4);
}

void RShaderProgram::setUniform(RUniformLocation location, GLuint v1) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid location!")) return;
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid shader program!")) return;
    if(printError(usingProgramID != *progID_, "Setting uniform " + location.name_
                  + " is invalid for " + nameID() + ", Its not have use!")) return;
#endif
    glUniform1ui(location, v1);
}

void RShaderProgram::setUniform(RUniformLocation location, GLuint v1, GLuint v2) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid location!")) return;
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid shader program!")) return;
    if(printError(usingProgramID != *progID_, "Setting uniform " + location.name_
                  + " is invalid for " + nameID() + ", Its not have use!")) return;
#endif
    glUniform2ui(location, v1, v2);
}

void RShaderProgram::setUniform(RUniformLocation location, GLuint v1, GLuint v2, GLuint v3) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid location!")) return;
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid shader program!")) return;
    if(printError(usingProgramID != *progID_, "Setting uniform " + location.name_
                  + " is invalid for " + nameID() + ", Its not have use!")) return;
#endif
    glUniform3ui(location, v1, v2, v3);
}

void RShaderProgram::setUniform(RUniformLocation location, GLuint v1, GLuint v2, GLuint v3, GLuint v4) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid location!")) return;
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid shader program!")) return;
    if(printError(usingProgramID != *progID_, "Setting uniform " + location.name_
                  + " is invalid for " + nameID() + ", Its not have use!")) return;
#endif
    glUniform4ui(location, v1, v2, v3, v4);
}

void RShaderProgram::setUniform(RUniformLocation location, GLsizei size, GLfloat *vp, GLsizei count) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid location!")) return;
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid shader program!")) return;
    if(printError(usingProgramID != *progID_, "Setting uniform " + location.name_
                  + " is invalid for " + nameID() + ", Its not have use!")) return;
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
        printError("Set Invalid size argument for Uniform" + std::to_string(size) + "fv"
                   + "In " + nameID());
        break;
    }
}

void RShaderProgram::setUniform(RUniformLocation location, GLsizei size, GLint *vp, GLsizei count) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid location!")) return;
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid shader program!")) return;
    if(printError(usingProgramID != *progID_, "Setting uniform " + location.name_
                  + " is invalid for " + nameID() + ", Its not have use!")) return;
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
        printError("Invalid size argument for Uniform" + std::to_string(size) + "iv"
                   + "In " + nameID());
        break;
    }
}

void RShaderProgram::setUniform(RUniformLocation location, GLsizei size, GLuint *vp, GLsizei count) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid location!")) return;
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid shader program!")) return;
    if(printError(usingProgramID != *progID_, "Setting uniform " + location.name_
                  + " is invalid for " + nameID() + ", Its not have use!")) return;
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
        printError("Invalid size argument for Uniform" + std::to_string(size) + "uiv"
                   + "In " + nameID());
        break;
    }
}

void RShaderProgram::setUniformMatrix(RUniformLocation location, GLsizei size, GLfloat *vp, GLsizei count, GLboolean transpose) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid location!")) return;
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid shader program!")) return;
    if(printError(usingProgramID != *progID_, "Setting uniform " + location.name_
                  + " is invalid for " + nameID() + ", Its not have use!")) return;
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
        printError("Invalid size argument for UniformMatrix" + std::to_string(size) + "fv"
                   + "In " + nameID());
        break;
    }
}

void RShaderProgram::setUniformMatrix(RUniformLocation location, GLsizei size, GLdouble *vp, GLsizei count, GLboolean transpose) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid location!")) return;
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid shader program!")) return;
    if(printError(usingProgramID != *progID_, "Setting uniform " + location.name_
                  + " is invalid for " + nameID() + ", Its not have use!")) return;
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
        printError("Invalid size argument for UniformMatrix" + std::to_string(size) + "fv"
                + "In " + nameID());
        break;
    }
}

void RShaderProgram::setUniformMatrix(RUniformLocation location, GLsizei column, GLsizei row, GLfloat *vp, GLsizei count, GLboolean transpose) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid location!")) return;
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid shader program!")) return;
    if(printError(usingProgramID != *progID_, "Setting uniform " + location.name_
                  + " is invalid for " + nameID() + ", Its not have use!")) return;
#endif
    switch(column) {
    case 2:
        if(row == 3){ glUniformMatrix2x3fv(location, count, transpose, vp); return; }
        if(row == 4){ glUniformMatrix2x4fv(location, count, transpose, vp); return; }
        break;
    case 3:
        if(row == 2){ glUniformMatrix3x2fv(location, count, transpose, vp); return; }
        if(row == 4){ glUniformMatrix3x4fv(location, count, transpose, vp); return; }
        break;
    case 4:
        if(row == 2){ glUniformMatrix4x2fv(location, count, transpose, vp); return; }
        if(row == 3){ glUniformMatrix4x3fv(location, count, transpose, vp); return; }
        break;
    }
#ifdef R_DEBUG
    printError(row < 2 || row > 4 || column < 2 || column > 4 || row == column,
               "Invalid size argument for UniformMatrix" + std::to_string(column) + 'x' + std::to_string(row)
               + "In " + nameID());
#endif
}

void RShaderProgram::setUniformMatrix(RUniformLocation location, GLsizei column, GLsizei row, GLdouble *vp, GLsizei count, GLboolean transpose) const
{
#ifdef R_DEBUG
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid location!")) return;
    if(printError(!progID_, "Setting uniform " + location.name_ + " is invalid for "
                  + nameID() + ", Its invalid shader program!")) return;
    if(printError(usingProgramID != *progID_, "Setting uniform " + location.name_
                  + " is invalid for " + nameID() + ", Its not have use!")) return;
    printError(row < 2 || row > 4 || column < 2 || column > 4 || row == column,
               "Invalid size argument for UniformMatrix" + std::to_string(column) + 'x' + std::to_string(row)
               + "In " + nameID());
#endif
    switch(column) {
    case 2:
        if(row == 3){ glUniformMatrix2x3dv(location, count, transpose, vp); return; }
        if(row == 4){ glUniformMatrix2x4dv(location, count, transpose, vp); return; }
        break;
    case 3:
        if(row == 2){ glUniformMatrix3x2dv(location, count, transpose, vp); return; }
        if(row == 4){ glUniformMatrix3x4dv(location, count, transpose, vp); return; }
        break;
    case 4:
        if(row == 2){ glUniformMatrix4x2dv(location, count, transpose, vp); return; }
        if(row == 3){ glUniformMatrix4x3dv(location, count, transpose, vp); return; }
        break;
    }
#ifdef R_DEBUG
    printError(row < 2 || row > 4 || column < 2 || column > 4 || row == column,
               "Invalid size argument for UniformMatrix" + std::to_string(column) + 'x' + std::to_string(row)
               + "In " + nameID());
#endif
}

void RShaderProgram::deleteShaderProgram(GLuint *ID)
{
    glDeleteProgram(*ID);
    delete ID;
}
