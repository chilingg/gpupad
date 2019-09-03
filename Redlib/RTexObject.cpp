#include "RTexObject.h"

RTexObject::RTexObject(int widht, int height, const char *path):
    RObject(widht, height),
    texture(RImage(path, true))
{
}

void RTexObject::allocation()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    float *plant = getPlantArray(_widht, _height);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*plant)*24, plant, GL_STATIC_DRAW);
    delete [] plant;

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), reinterpret_cast<void*>(2*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

float *RTexObject::getPlantArray(int widht, int height)
{
    float *plant = new float[24]{
            0.0f, 0.0f, 0.0f, 0.0f,//左下
            0.0f, static_cast<float>(height), 0.0f, 1.0f,//左上
            static_cast<float>(widht), static_cast<float>(height), 1.0f, 1.0f,//右上
            static_cast<float>(widht), static_cast<float>(height), 1.0f, 1.0f,//右上
            static_cast<float>(widht), 0.0f, 1.0f, 0.0f,//右下
            0.0f, 0.0f, 0.0f, 0.0f,//左下
    };

    return plant;
}

void RTexObject::renderControl(RShaderProgram *shader)
{
    texture.bind();
}
