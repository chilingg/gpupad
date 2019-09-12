#ifndef RSPRITE_H
#define RSPRITE_H

#include <RTexObject.h>

class RSprite : public RTexObject
{
public:
    RSprite(int width, int height);
    bool setCurrentTextureArray();
};

#endif // RSPRITE_H
