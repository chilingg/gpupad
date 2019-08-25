#ifndef ROBJECT_H
#define ROBJECT_H

#include <RShaderProgram.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class RObject
{
public:
    RObject(int widht, int height);
    virtual ~RObject();
    virtual void render(RShaderProgram *shader);

    void setPosition(int x, int y);
    void move(glm::vec2 direction, int step);
    void setColor(int r, int g, int b, int a = 255);

protected:
    static float* getPlantArray(int widht, int height);

    glm::vec2 pos;
    glm::vec4 color;
    int widht;
    int height;
    unsigned VAO, VBO;
};

inline void RObject::setPosition(int x, int y)
{
    pos.x = static_cast<float>(x);
    pos.y = static_cast<float>(y);
}

inline void RObject::move(glm::vec2 direction, int step)
{
    glm::vec2 velocity = glm::normalize(direction) * static_cast<float>(step);
    pos += velocity;
}

inline void RObject::setColor(int r, int g, int b, int a)
{
    color.r = r/255.0f;
    color.g = g/255.0f;
    color.b = b/255.0f;
    color.a = a/255.0f;
}

#endif // ROBJECT_H
