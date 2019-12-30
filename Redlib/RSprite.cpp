#include "RSprite.h"

RSprite::RSprite():
    RSprite(32, 32, "Sprite", RPoint(0, 0))
{

}

RSprite::RSprite(const RSprite &sprite):
    RPlane(sprite),
    textures_(sprite.textures_),
    interval_(sprite.interval_),
    delta_(sprite.delta_),
    textureIndex_(sprite.textureIndex_)
{

}

RSprite::RSprite(int width, int height, const std::string &name, RPoint pos):
    RPlane(width, height, name, pos)
{

}

RSprite::~RSprite()
{

}

size_t RSprite::getFrameCount()
{
    return textures_.size();
}

size_t RSprite::getCurrentFrameIndex()
{
    return textureIndex_;
}

void RSprite::setInterval(int interval)
{
    interval_ = interval;
    delta_ = 0;
}

size_t RSprite::addFrame(const RTexture &frame)
{
    textures_.push_back(frame);
    texture_ = frame;
    textureIndex_ = textures_.size();
    return textures_.size();
}

void RSprite::play()
{
    render();

    if(delta_ < interval_)
        ++delta_;
    else {
        delta_ = 0;

        if(!textures_.empty())
        {
            textureIndex_ = ++textureIndex_ % textures_.size();
            texture_ = textures_[textureIndex_];
        }
    }
}

bool RSprite::setFrame(size_t n)
{
    if(n > textures_.size())
        return false;
    texture_ = textures_[n];
    textureIndex_ = n;
    return true;
}

void RSprite::clearFrameSequence()
{
    std::vector<RTexture> textures;
    textures_.swap(textures);
}
