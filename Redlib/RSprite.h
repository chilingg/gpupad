#ifndef RSPRITE_H
#define RSPRITE_H

#include <RTexObject.h>
#include <vector>

class RSprite : public RTexObject
{
public:
    RSprite(int width, int height);

    bool setCurrentTextureArray(std::vector<std::string> sequence, int interval = 20);
    void setInterval(int interval);

protected:
    void renderControl(RShaderProgram *shader) override;

private:
    std::vector<std::string> currentTexAry;
    int _interval;
    int delta;
    std::vector<std::string>::size_type texIndex;
};

inline void RSprite::setInterval(int interval)
{
    _interval = interval;
    delta = interval;
    texIndex = 0;
}

#endif // RSPRITE_H
