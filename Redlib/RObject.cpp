#include "RObject.h"

RObject::RObject(int widht, int height):
    pos(0.0f, 0.0f),
    color(1.0f),
    widht(widht),
    height(height)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    float *plant = getPlantArray(widht, height);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*plant)*12, plant, GL_STATIC_DRAW);
    delete [] plant;

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

RObject::~RObject()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void RObject::render(RShaderProgram *shader)
{
    glBindVertexArray(VAO);

    shader->use();
    shader->setUniform4F("color", color);

    glm::mat4 model(1.0f);
    model = glm::translate(model, {pos, 0.0f});
    shader->setUniformMatrix4fv("model", glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

float *RObject::getPlantArray(int widht, int height)
{
    float *plant = new float[12]{
            0.0f, 0.0f,//左下
            0.0f, static_cast<float>(height),//左上
            static_cast<float>(widht), static_cast<float>(height),//右上
            static_cast<float>(widht), static_cast<float>(height),//右上
            static_cast<float>(widht), 0.0f,//右下
            0.0f, 0.0f,//左下
    };

    return plant;
}
