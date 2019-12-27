#ifndef LEARN_H
#define LEARN_H

#include <RController.h>
#include <RMaths.h>
#include <RResource/RShaderProgram.h>
#include <RPlane.h>

class Learn : public RController
{
public:
    explicit Learn(const std::string &name = "Controller", RController *parent = nullptr);
    ~Learn() override;

    void control() override;

protected:
    void startEvent(RStartEvent *event) override;

private:
    static const char render_vs[];

    RShaderProgram render_prog;

    GLuint fbo;
    GLuint render_vao;
    GLuint render_vbo;
    GLint inputAttrib;

};

#endif // LEARN_H
