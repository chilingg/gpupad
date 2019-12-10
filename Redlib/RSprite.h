#ifndef RSPRITE_H
#define RSPRITE_H

#include "RPlane.h"
#include <vector>

class RSprite : public RPlane
{
public:
    explicit RSprite();
    RSprite(const RSprite &sprite);
    RSprite(int width, int height, const std::string &name, RPoint pos);
    virtual ~RSprite() override;

    void setInterval(int interval = 20);
    void addFrame(const RTexture &frame);
    void clearFrameSequence();

protected:
    void renderControl() override;

private:
    std::vector<RTexture> textures_;
    int interval_ = 20;
    int delta_ = 0;
    size_t textureIndex_ = 0;
    bool played_ = false;
};

#endif // RSPRITE_H
