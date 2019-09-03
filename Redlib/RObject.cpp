#include "RObject.h"

RObject::RObject(int widht, int height):
    _pos(0.0f, 0.0f),
    velocity(0.0f, 0.0f),
    color(1.0f),
    _widht(widht),
    _height(height),
    volume(&_pos, widht, height)
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

bool RObject::touchSide(const RObject &platform, RVolume::Side side, int extend) const
{
    bool b = false;
    switch(side)
    {
    case RVolume::Top:
        b = volume.checkCollision(platform.volume, true, false, extend) && volume.containsAxisY(platform.volume.top());
        break;
    case RVolume::Bottom:
        b = volume.checkCollision(platform.volume, true, false, extend) && volume.containsAxisY(platform.volume.bottom());
        break;
    case RVolume::Left:
        b = volume.checkCollision(platform.volume, false, true, extend) && volume.containsAxisX(platform.volume.left());
        break;
    case RVolume::Right:
        b = volume.checkCollision(platform.volume, false, true, extend) && volume.containsAxisX(platform.volume.right());
        break;
    default:
        break;
    }
    return b;
}

void RObject::render(RShaderProgram *shader)
{
    glBindVertexArray(VAO);
    shader->use();
    renderControl(shader);

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

void RObject::renderControl(RShaderProgram *shader)
{
    shader->setUniform4F("color", color);

    glm::mat4 model(1.0f);
    model = glm::translate(model, {_pos, 0.0f});
    shader->setUniformMatrix4fv("model", glm::value_ptr(model));
}
