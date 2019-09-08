#ifndef RVOLUME_H
#define RVOLUME_H

#include <glm/vec2.hpp>
#include <RSize.h>
#include <RDebug.h>

class RVolume
{
public:
    enum Side{
        Top,
        Bottom,
        Left,
        Right
    };

    RVolume(glm::vec2 pos, int width = 0, int height = 0);
    RVolume(glm::vec2 pos, RSize size);

    void setSize(int width, int height);
    void setWidth(int width);
    void setHeight(int height);
    void setPos(glm::vec2 pos);

    int left() const;
    int right() const;
    int top() const;
    int bottom() const;
    float leftF() const;
    float rightF() const;
    float topF() const;
    float bottomF() const;
    int width() const;
    int height() const;
    float &rx();
    float& ry();
    glm::vec2 getPos() const;
    RSize getSize() const;

    bool checkCollision(const RVolume &volume, bool left = true, bool right = true, int extend = 0) const;
    bool contains(const RVolume &volume, bool top = true, bool bottom = true, bool left = true, bool right = true) const;
    bool containsAxisX(float left) const;
    bool containsAxisY(float right) const;

private:
    glm::vec2 _pos;
    float _width;
    float _height;
};

inline RVolume::RVolume(glm::vec2 pos, int width, int height):
    _pos(pos),
    _width(width),
    _height(height)
{

}

inline RVolume::RVolume(glm::vec2 pos, RSize size):
    RVolume(pos, size.width(), size.height())
{

}

inline void RVolume::setSize(int width, int height)
{
    _width = width;
    _height = height;
}

inline void RVolume::setWidth(int width)
{
    _width = width;
}

inline void RVolume::setHeight(int height)
{
    _height = height;
}

inline void RVolume::setPos(glm::vec2 pos)
{
    _pos = pos;
}

inline int RVolume::left() const
{
    return static_cast<int>(_pos.x);
}

inline int RVolume::right() const
{
    return static_cast<int>(_pos.x + _width);
}

inline int RVolume::top() const
{
    return static_cast<int>(_pos.y + _height);
}

inline int RVolume::bottom() const
{
    return static_cast<int>(_pos.y);
}

inline float RVolume::leftF() const
{
    return _pos.x;
}

inline float RVolume::rightF() const
{
    return _pos.x + _width;
}

inline float RVolume::topF() const
{
    return _pos.y + _height;
}

inline float RVolume::bottomF() const
{
    return _pos.y;
}

inline int RVolume::width() const
{
    return static_cast<int>(_width);
}

inline int RVolume::height() const
{
    return static_cast<int>(_height);
}

inline float &RVolume::rx()
{
    return _pos.x;
}

inline float &RVolume::ry()
{
    return _pos.y;
}

inline glm::vec2 RVolume::getPos() const
{
    return _pos;
}

inline RSize RVolume::getSize() const
{
    return RSize(_width, _height);
}

inline bool RVolume::checkCollision(const RVolume &volume, bool x, bool y, int extend) const
{
    bool b = true;
    //RDebug() << _pos.y + _height << volume._pos.y - extend << _pos.y << volume._pos.y + volume._height + extend;
    if(x)
        b &= _pos.x + _width >= volume._pos.x - extend && _pos.x <= volume._pos.x + volume._width + extend;
    if(y)
        b &= _pos.y + _height >= volume._pos.y - extend && _pos.y <= volume._pos.y + volume._height + extend;

    return b;
}

inline bool RVolume::contains(const RVolume &volume, bool top, bool bottom, bool left, bool right) const
{
    bool b = true;

    if(top)
        b &= _pos.y + _height >= volume._pos.y + volume._height;
    if(bottom)
        b &= _pos.y <= volume._pos.y;
    if(left)
        b &= _pos.x <= volume._pos.x;
    if(right)
        b &= _pos.x + _width >= volume._pos.x + volume._width;

    return b;
}

inline bool RVolume::containsAxisX(float x) const
{
    return _pos.x + _width >= x && _pos.x <= x;
}

inline bool RVolume::containsAxisY(float y) const
{
    return _pos.y + _height >= y && _pos.y <= y;
}

#endif // RVOLUME_H
