#include "RTexObject.h"

unsigned RTexObject::texVAO(0);
unsigned RTexObject::texVBO(0);

RTexObject::RTexObject(int width, int height):
    RObject(width, height),
    textures()
{
    if(texVAO == 0)
        allocation();
}

void RTexObject::render(RShaderProgram *shader)
{
    glBindVertexArray(texVAO);
    shader->use();
    renderControl(shader);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void RTexObject::allocation()
{
    glGenVertexArrays(1, &texVAO);
    glGenBuffers(1, &texVBO);

    glBindVertexArray(texVAO);
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);

    float plant[24]{
            0.0f, 0.0f, 0.0f, 0.0f,//左下
            0.0f, 1.0f, 0.0f, 1.0f,//左上
            1.0f, 1.0f, 1.0f, 1.0f,//右上
            1.0f, 1.0f, 1.0f, 1.0f,//右上
            1.0f, 0.0f, 1.0f, 0.0f,//右下
            0.0f, 0.0f, 0.0f, 0.0f,//左下
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(plant), plant, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), reinterpret_cast<void*>(2*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void RTexObject::renderControl(RShaderProgram *shader)
{
    RObject::renderControl(shader);
    textures[currentTex].bind();
}

void RTexObject::updataSizeMat()
{
    int tw, th;
    switch(_sizePattern) {
    case Length:
        tw = textures[currentTex].width();
        th = textures[currentTex].height();
        break;
    case Auto:
        tw = _width;
        th = _height;
        break;
    case Cover:
        int base = textures[currentTex].width() / _width;
        tw = _width;
        th = _height;
        break;
    case Auto:
        tw = _width;
        th = _height;
        break;
    }

    sizeMat[0][0] = _width - _paddingLeft - _paddingRight;
    sizeMat[1][1] = _height - _paddingTop - _paddingBottom;

    sizeMat[3][0] = _paddingLeft;
    sizeMat[3][1] = _paddingBottom;
}
