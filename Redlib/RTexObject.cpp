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
            1.0f, 0.0f, 1.0f, 0.0f,//右下
            1.0f, 1.0f, 1.0f, 1.0f,//右上
            1.0f, 1.0f, 1.0f, 1.0f,//右上
            0.0f, 1.0f, 0.0f, 1.0f,//左上
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
    int width = _width - _paddingLeft - _paddingRight;
    int height = _height - _paddingTop - _paddingBottom;
    int min = textures[currentTex].width() / width;
    int max = textures[currentTex].height() / height;
    if(min > max)
        std::swap(min, max);
    switch(_sizePattern) {
    case Length:
        tw = textures[currentTex].width();
        th = textures[currentTex].height();
        break;
    case Auto:
        tw = width;
        th = height;
        break;
    case Cover:
        tw = textures[currentTex].width() * max;
        th = textures[currentTex].height() * max;
        break;
    case Contain:
        tw = textures[currentTex].width() * min;
        th = textures[currentTex].height() * min;
        break;
    }
    sizeMat[0][0] = tw;
    sizeMat[1][1] = th;

    if(_hAlign == Align_Left)
        sizeMat[3][0] = _paddingLeft;
    else if(_hAlign == Align_Mind)
        sizeMat[3][0] = _width/2 - tw/2;
    else if(_hAlign == Align_Right)
        sizeMat[3][0] = _width - tw - _paddingRight;
    if(_vAlign == Align_Bottom)
        sizeMat[3][1] = _paddingBottom;
    else if(_vAlign == Align_Mind)
        sizeMat[3][1] = _height/2 - th/2;
    else if(_vAlign == Align_Top)
        sizeMat[3][1] = _height - th - _paddingTop;

    sizeMat[3][0] += _marginLeft;
    sizeMat[3][1] += _marginBottom;

    glm::mat4 flipMat(1);
    if(_flipH == true)
    {
        flipMat[3][0] = 1;
        flipMat[0][0] = -1;
    }
    if(_flipV == true)
    {
        flipMat[3][1] = 1;
        flipMat[1][1] = -1;
    }
    sizeMat *= flipMat;
}
