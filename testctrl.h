#ifndef TESTCTRL_H
#define TESTCTRL_H

#include <RShaderProgram.h>
#include <RController.h>
#include <glm/mat4x4.hpp>

class TestCtrl : public RController
{
public:
    TestCtrl(RController *parent = nullptr);
    ~TestCtrl() override;

protected:
    void paintEvent() override;
    void keyPressEvent(RKeyEvent *event) override;
    void keyReleaseEvent(RKeyEvent *event) override;
    void mousePressEvent(RMouseEvent *event) override;
    void mouseReleaseEvent(RMouseEvent *event) override;

private:
    const float VIEW_PROT_WIDTH;
    const float VIEW_PROT_HEIGHT;
    RShaderProgram program;
    unsigned VAO, VBO;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 project;
};

#endif // TESTCTRL_H
