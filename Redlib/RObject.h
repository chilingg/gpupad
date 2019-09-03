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
    RObject(const RObject&) = delete;
    RObject& operator=(const RObject&) = delete;
    virtual ~RObject();
    virtual void allocation();

    void setColor(int r, int g, int b, int a = 255);
    void setPosition(int x, int y);
    void setPositionX(int x);
    void setPositionY(int y);
    void setVelocity(int x, int y);
    void setVelocityX(int x);
    void setVelocityX(float x);
    void setVelocityY(float y);
    void setVelocityY(int y);
    void setVelocity(glm::vec2 velocity);

    const glm::vec2& getVelocity() const;
    int widht() const;
    int height() const;
    glm::vec2 pos() const;
    int x() const;
    float& rx();
    int y() const;
    float& ry();

    void render(RShaderProgram *shader);
    void giveVelocity(int x, int y);
    void powerVelocity(double value);
    void stop();
    void motion(bool b = true);
    void move(glm::vec2 direction, int step);
    bool checkCollision(const RObject &obj) const;
    const RVolume& getVolume() const;
    bool touchSide(const RObject & platform, RVolume::Side side, int extend = 0) const;

protected:
    virtual void renderControl(RShaderProgram *shader);

    float* getPlantArray(int widht, int height);

    glm::vec2 _pos;
    glm::vec2 velocity;
    glm::vec4 color;
    int _widht;
    int _height;
    RVolume volume;

    unsigned VAO, VBO;
};

inline void RObject::setPosition(int x, int y)
{
    _pos.x = static_cast<float>(x);
    _pos.y = static_cast<float>(y);
}

inline void RObject::setPositionX(int x)
{
    _pos.x = x;
}

inline void RObject::setPositionY(int y)
{
    _pos.y = y;
}

inline void RObject::move(glm::vec2 direction, int step)
{
    if(direction.x == 0.0f && direction.y == 0.0f)
        return;
    glm::vec2 velocity = glm::normalize(direction) * static_cast<float>(step);
    _pos += velocity;
}

inline void RObject::setColor(int r, int g, int b, int a)
{
    color.r = r/255.0f;
    color.g = g/255.0f;
    color.b = b/255.0f;
    color.a = a/255.0f;
}

inline bool RObject::checkCollision(const RObject &obj) const
{
    return volume.checkCollision(obj.volume);
}

inline const RVolume &RObject::getVolume() const
{
    return volume;
}

inline void RObject::setVelocity(int x, int y)
{
    setVelocity({x, y});
}

inline void RObject::setVelocityX(int x)
{
    velocity.x = x;
}

inline void RObject::setVelocityX(float x)
{
    velocity.x = x;
}

inline void RObject::setVelocityY(float y)
{
    velocity.y = y;
}

inline void RObject::setVelocityY(int y)
{
    velocity.y = y;
}

inline void RObject::setVelocity(glm::vec2 velocity)
{
    this->velocity = velocity;
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

inline int RObject::widht() const
{
    return _widht;
}

inline int RObject::height() const
{
    return _height;
}

inline glm::vec2 RObject::pos() const
{
    return _pos;
}

inline int RObject::x() const
{
    return static_cast<int>(_pos.x);
}

inline float &RObject::rx()
{
    return _pos.x;
}

inline int RObject::y() const
{
    return static_cast<int>(_pos.y);
}

inline float &RObject::ry()
{
    return _pos.y;
}

inline void RObject::stop()
{
    velocity = {0, 0};
}

inline void RObject::motion(bool b)
{
    _pos += b ? velocity : -velocity;
}

#endif // ROBJECT_H
