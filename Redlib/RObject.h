#ifndef ROBJECT_H
#define ROBJECT_H

#include <RShaderProgram.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <RVolume.h>
#include <vector>

class RObject
{
public:
    RObject(int width, int height);
    RObject(const RObject&) = delete;
    RObject& operator=(const RObject&) = delete;
    virtual ~RObject();
    virtual void render(RShaderProgram *shader);

    void setColor(int r, int g, int b, int a = 255);
    void setPosition(int x, int y);
    void setPosition(glm::vec2 pos);
    void setPositionX(int x);
    void setPositionY(int y);
    void setVelocity(int x, int y);
    void setVelocityX(int x);
    void setVelocityX(float x);
    void setVelocityY(float y);
    void setVelocityY(int y);
    void setVelocity(glm::vec2 velocity);
    void setMargin(int top, int bottom, int left, int right);
    void setMargin(int value);
    void setPadding(int top, int bottom, int left, int right);
    void setPadding(int value);

    const glm::vec2& getVelocity() const;
    int width() const;
    int height() const;
    glm::vec2 pos() const;
    int x() const;
    float& rx();
    int y() const;
    float& ry();
    RVolume volume() const;
    bool isFlipV() const;
    bool isFlipH() const;

    bool moveCollision(glm::vec2 &velocity, const RObject &platform);
    void giveVelocity(int x, int y);
    void powerVelocity(double value);
    void stop();
    void motion(bool b = true);
    void move(glm::vec2 direction, int step);
    bool checkCollision(const RObject &obj) const;
    bool touchSide(const RObject & platform, RVolume::Side side, int extend = 0) const;
    void displayVolume(const glm::mat4 &projection, const glm::mat4 &view);
    void flip(bool h, bool v = false);

protected:
    static RShaderProgram *volumeShader;
    static unsigned vVAO;
    static unsigned plantVAO, plantVBO;

    virtual void renderControl(RShaderProgram *shader);

    void setSizeMat();

    glm::vec2 _pos;
    glm::vec2 velocity;
    glm::vec4 color;
    int _width;
    int _height;
    glm::mat4 sizeMat;

    bool _flipH = false;
    bool _flipV = false;

    int _marginTop = 0;
    int _marginBottom = 0;
    int _marginLeft = 0;
    int _marginRight = 0;

    int _paddingTop = 0;
    int _paddingBottom = 0;
    int _paddingLeft = 0;
    int _paddingRight = 0;

private:
    void allocation();
};

inline void RObject::setPosition(int x, int y)
{
    _pos.x = static_cast<float>(x);
    _pos.y = static_cast<float>(y);
}

inline void RObject::setPosition(glm::vec2 pos)
{
    _pos = pos;
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
    return volume().checkCollision(obj.volume());
}

inline void RObject::flip(bool h, bool v)
{
    _flipH = h;
    _flipV = v;
}

inline RVolume RObject::volume() const
{
    return {{_pos.x-_marginLeft, _pos.y-_marginBottom}, _width+_marginRight, _height+_marginTop};
}

inline bool RObject::isFlipV() const
{
    return _flipV;
}

inline bool RObject::isFlipH() const
{
    return _flipH;
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

inline void RObject::setMargin(int top, int bottom, int left, int right)
{
    _marginTop = top;
    _marginBottom = bottom;
    _marginLeft = left;
    _marginRight = right;
}

inline void RObject::setMargin(int value)
{
    setMargin(value, value, value, value);
}

inline void RObject::setPadding(int top, int bottom, int left, int right)
{
    _paddingTop = top;
    _paddingBottom = bottom;
    _paddingLeft = left;
    _paddingRight = right;
}

inline void RObject::setPadding(int value)
{
    setPadding(value, value, value, value);
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

inline int RObject::width() const
{
    return _width;
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
