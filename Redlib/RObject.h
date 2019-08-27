#ifndef ROBJECT_H
#define ROBJECT_H

#include <RShaderProgram.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <RVolume.h>

class RObject
{
public:
    RObject(int widht, int height);
    virtual ~RObject();
    virtual void render(RShaderProgram *shader);

    void setColor(int r, int g, int b, int a = 255);
    void setPosition(int x, int y);
    void setVelocity(int x, int y);

    const glm::vec2& getVelocity() const;
    int x() const;
    int y() const;

    void giveVelocity(int x, int y);
    void powerVelocity(double value);
    void stop();
    void move(glm::vec2 direction, int step);
    bool checkCollision(const RObject &obj);
    const RVolume& getVolume() const;

protected:
    static float* getPlantArray(int widht, int height);

    glm::vec2 pos;
    glm::vec2 velocity;
    glm::vec4 color;
    int widht;
    int height;
    RVolume volume;

    unsigned VAO, VBO;
};

inline void RObject::setPosition(int x, int y)
{
    pos.x = static_cast<float>(x);
    pos.y = static_cast<float>(y);
}

inline void RObject::move(glm::vec2 direction, int step)
{
    if(direction.x == 0.0f && direction.y == 0.0f)
        return;
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

inline bool RObject::checkCollision(const RObject &obj)
{
    return volume.checkCollision(obj.volume);
}

inline const RVolume &RObject::getVolume() const
{
    return volume;
}

inline void RObject::setVelocity(int x, int y)
{
    velocity = {x, y};
}

inline void RObject::giveVelocity(int x, int y)
{
    velocity.x += x;
    velocity.y += y;
}

inline void RObject::powerVelocity(double value)
{
    velocity *= value;
}

inline const glm::vec2 &RObject::getVelocity() const
{
    return velocity;
}

inline int RObject::x() const
{
    return static_cast<int>(pos.x);
}

inline int RObject::y() const
{
    return static_cast<int>(pos.y);
}

inline void RObject::stop()
{
    velocity = {0, 0};
}

#endif // ROBJECT_H
