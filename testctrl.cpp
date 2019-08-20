#include "testctrl.h"
#include "constant.h"
#include <RDebug.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TestCtrl::TestCtrl(RController *parent):
    RController(parent),
    VIEW_PROT_WIDTH(1920.0f),
    VIEW_PROT_HEIGHT(1080.0f)
{
    RDebug() << 1;
    RShader vertex((RE_PATH + "vertex.vert"), RShader::VERTEX_SHADER);
    RShader fragment((RE_PATH + "fragment.frag"), RShader::FRAGMENT_SHADER);
    program.attachShader(vertex);
    program.attachShader(fragment);
    program.linkProgram();

    float plant[] = {
        -1.0f, 1.0f, .0f, .5f, .5f, .5f,
        1.0f, 1.0f, .0f, .5f, .5f, .5f,
        -1.0f, -1.0f, .0f, .5f, .5f, .5f,
        -1.0f, -1.0f, .0f, .5f, .5f, .5f,
        1.0f, 1.0f, .0f, .5f, .5f, .5f,
        1.0f, -1.0f, .0f, .5f, .5f, .5f,
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plant), plant, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    project = glm::ortho(0.0f, VIEW_PROT_WIDTH, 0.0f, VIEW_PROT_HEIGHT, -1.0f, 1.0f);

    view = glm::mat4(1);

    model = glm::mat4(1);
    model = glm::scale(model, {0.4f, 0.4f, 0.0f});
}

TestCtrl::~TestCtrl()
{

}

void TestCtrl::paintEvent()
{
    glBindVertexArray(VAO);
    program.use();
    program.setUniform3F("color", 1.0f, 1.0f, 1.0f);
    program.setUniformMatrix4fv("model", glm::value_ptr(model));
    program.setUniformMatrix4fv("view", glm::value_ptr(view));
    program.setUniformMatrix4fv("project", glm::value_ptr(project));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void TestCtrl::keyPressEvent(RKeyEvent *event)
{
    //RDebug() << event->key();
}

void TestCtrl::keyReleaseEvent(RKeyEvent *event)
{
    //RDebug() << event->key();
}

void TestCtrl::mousePressEvent(RMouseEvent *event)
{
    //RDebug() << event->x() << event->y();
}

void TestCtrl::mouseReleaseEvent(RMouseEvent *event)
{
    //RDebug() << event->x() << event->y();
}
