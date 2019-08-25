#ifndef RVOLUME_H
#define RVOLUME_H

#include <glm/vec2.hpp>
#include <RDebug.h>

class RVolume
{
public:
    RVolume(glm::vec2 *pos, int widht = 0, int height = 0);
    void setSize(int widht, int height);
    bool checkCollision(const RVolume &volume);

private:
    glm::vec2 *_pos;
    int _widht;
    int _height;
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

inline bool RVolume::checkCollision(const RVolume &volume)
{
    bool collitionX = _pos->x + _widht >= volume._pos->x && _pos->x <= volume._pos->x + volume._widht;
    bool collitionY = _pos->y + _height >= volume._pos->y && _pos->y <= volume._pos->y + volume._height;

    return collitionX && collitionY;
}

#endif // RVOLUME_H
