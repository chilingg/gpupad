#ifndef RVOLUME_H
#define RVOLUME_H

#include <glm/vec2.hpp>
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

    RVolume(glm::vec2 *pos, int widht = 0, int height = 0);

    void setSize(int widht, int height);

    int left() const;
    int right() const;
    int top() const;
    int bottom() const;
    int widht() const;
    int height() const;

    bool checkCollision(const RVolume &volume, bool left = true, bool right = true, int extend = 0) const;
    bool contains(const RVolume &volume, bool top = true, bool bottom = true, bool left = true, bool right = true) const;
    bool containsAxisX(float left) const;
    bool containsAxisY(float right) const;

private:
    glm::vec2 *_pos;
    float _widht;
    float _height;
};

inline RVolume::RVolume(glm::vec2 *pos, int widht, int height):
    _pos(pos),
    _widht(widht),
    _height(height)
{

}

inline void RVolume::setSize(int widht, int height)
{
    _widht = widht;
    _height = height;
}

inline int RVolume::left() const
{
    return static_cast<int>(_pos->x);
}

inline int RVolume::right() const
{
    return static_cast<int>(_pos->x + _widht);
}

inline int RVolume::top() const
{
    return static_cast<int>(_pos->y + _height);
}

inline int RVolume::bottom() const
{
    return static_cast<int>(_pos->y);
}

inline int RVolume::widht() const
{
    return static_cast<int>(_widht);
}

inline int RVolume::height() const
{
    return static_cast<int>(_height);
}

inline bool RVolume::checkCollision(const RVolume &volume, bool x, bool y, int extend) const
{
    bool b = true;
    //RDebug() << _pos->y + _height << volume._pos->y - extend << _pos->y << volume._pos->y + volume._height + extend;
    if(x)
        b &= _pos->x + _widht >= volume._pos->x - extend && _pos->x <= volume._pos->x + volume._widht + extend;
    if(y)
        b &= _pos->y + _height >= volume._pos->y - extend && _pos->y <= volume._pos->y + volume._height + extend;

    return b;
}

inline bool RVolume::contains(const RVolume &volume, bool top, bool bottom, bool left, bool right) const
{
    bool b = false;

    if(top)
        b |= _pos->y + _height >= volume._pos->y + volume._height;
    if(bottom)
        b |= _pos->y <= volume._pos->y;
    if(left)
        b |= _pos->x <= volume._pos->x;
    if(right)
        b |= _pos->x + _widht >= volume._pos->x + volume._widht;

    return b;
}

inline bool RVolume::containsAxisX(float x) const
{
    return _pos->x + _widht >= x && _pos->x <= x;
}

inline bool RVolume::containsAxisY(float y) const
{
    return _pos->y + _height >= y && _pos->y <= y;
}

#endif // RVOLUME_H
