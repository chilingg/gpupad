#include "RTexObject.h"

RTexObject::RTexObject(int width, int height):
    RObject(width, height),
    textures()
{

}

void RTexObject::allocation()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    float *plant = getPlantArray();
    glBufferData(GL_ARRAY_BUFFER, sizeof(*plant)*24, plant, GL_STATIC_DRAW);
    delete [] plant;

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), reinterpret_cast<void*>(2*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

float *RTexObject::getPlantArray()
{
    float wf = static_cast<float>(_width);
    float hf = static_cast<float>(_height);
    float pt = _paddingTop / hf;
    float pb = _paddingBottom / hf;
    float pl = _paddingLeft / wf;
    float pr = _paddingRight / wf;

    float *plant = new float[24]{
            0.0f, 0.0f, 0.0f-pl, 0.0f-pb,//左下
            0.0f, hf, 0.0f-pl, 1.0f+pt,//左上
            wf, hf, 1.0f+pr, 1.0f+pt,//右上
            wf, hf, 1.0f+pr, 1.0f+pt,//右上
            wf, 0.0f, 1.0f+pr, 0.0f-pb,//右下
            0.0f, 0.0f, 0.0f-pl, 0.0f-pb,//左下
    };

    return plant;
}

void RTexObject::renderControl(RShaderProgram *shader)
{
    RObject::renderControl(shader);
    textures[currentTex].bind();
}
