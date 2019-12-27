#include "Learn.h"
#include <RMaths.h>
#include <RTimer.h>
#include <RWindowCtrl.h>
#include <RDebug.h>

Learn::Learn(const std::string &name, RController *parent):
    RController(name, parent)
{

}

Learn::~Learn()
{

}

void Learn::control()
{
    glBeginTransformFeedback(GL_POINTS);
    glDrawArrays(GL_POINTS, 0, 5);
    glEndTransformFeedback();
    glFlush();

    GLfloat feedback[5];
    glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);

    RDebug() << feedback[0] << feedback[1] << feedback[2] << feedback[3] << feedback[4];
}

void Learn::startEvent(RStartEvent *event)
{
    render_prog.attachShaderCode(render_vs, ShaderType::VertexShader);

    const char * varyings[] = { "outValue" };

    render_prog.linkProgram();
    glTransformFeedbackVaryings(render_prog.resourceID(), 1, varyings, GL_INTERLEAVED_ATTRIBS);
    render_prog.relinkProgram();
    render_prog.use();

    glGenVertexArrays(1, &render_vao);
    glBindVertexArray(render_vao);

    GLfloat data[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
    glGenBuffers(1, &render_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, render_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    inputAttrib = glGetAttribLocation(render_prog.resourceID(), "inValue");
    glEnableVertexAttribArray(inputAttrib);
    glVertexAttribPointer(inputAttrib, 1, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &fbo);
    glBindBuffer(GL_ARRAY_BUFFER, fbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), nullptr, GL_STATIC_READ);

    glEnable(GL_RASTERIZER_DISCARD);//光栅化器禁用
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, fbo);
}

const char Learn::render_vs[] =
        "#version 410\n"

        "in float inValue;\n"
        "out float outValue;\n"

        "void main(void)\n"
        "{\n"
        "    outValue = sqrt(inValue);\n"
        "}\n";
