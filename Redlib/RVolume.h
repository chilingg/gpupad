#ifndef RVOLUME_H
#define RVOLUME_H

#include <glm/vec2.hpp>
#include <RDebug.h>

class RVolume
{
public:
    RVolume(glm::vec2 *pos, int widht = 0, int height = 0);
    void setSize(int widht, int height);
    int x() const;
    int y() const;
    bool checkCollision(const RVolume &volume) const;
    bool contains(const RVolume &volume) const;
    bool containsX(const RVolume &volume) const;
    bool containsY(const RVolume &volume) const;

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

inline int RVolume::x() const
{
    return static_cast<int>(_pos->x);
}

inline int RVolume::y() const
{
    return static_cast<int>(_pos->y);
}

inline bool RVolume::checkCollision(const RVolume &volume) const
{
    bool collitionX = _pos->x + _widht >= volume._pos->x && _pos->x <= volume._pos->x + volume._widht;
    bool collitionY = _pos->y + _height >= volume._pos->y && _pos->y <= volume._pos->y + volume._height;

    return collitionX && collitionY;
}

inline bool RVolume::contains(const RVolume &volume) const
{
    bool collitionX = _pos->x + _widht >= volume._pos->x + volume._widht && _pos->x <= volume._pos->x;
    bool collitionY = _pos->y + _height >= volume._pos->y + volume._height && _pos->y <= volume._pos->y;
    //RDebug() << *_pos << *volume._pos << collitionX;

    return collitionX && collitionY;
}

inline bool RVolume::containsX(const RVolume &volume) const
{
    return _pos->x + _widht >= volume._pos->x + volume._widht && _pos->x <= volume._pos->x;
}

inline bool RVolume::containsY(const RVolume &volume) const
{
    return _pos->y + _height >= volume._pos->y + volume._height && _pos->y <= volume._pos->y;
}

#endif // RVOLUME_H
