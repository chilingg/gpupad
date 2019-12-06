#include "RSprite.h"

RSprite::RSprite(RShaderProgram program, const std::string &name):
    RSprite(32, 32, name, RPoint(0, 0), program)
{

}

RSprite::RSprite(const RSprite &sprite):
    RPlane(sprite),
    textures_(sprite.textures_),
    interval_(sprite.interval_),
    delta_(sprite.delta_),
    textureIndex_(sprite.textureIndex_),
    played_(sprite.played_)
{

}

RSprite::RSprite(int width, int height, const std::string &name, RPoint pos, RShaderProgram program):
    RPlane(width, height, name, pos, program)
{

}

RSprite::~RSprite()
{

}

void RSprite::setInterval(int interval)
{
    interval_ = interval;
    delta_ = 0;
}

void RSprite::addFrame(const RTexture &frame)
{
    textures_.push_back(frame);
    texture_ = frame;
}

void RSprite::clearFrameSequence()
{
    std::vector<RTexture> textures;
    textures_.swap(textures);
}

void RSprite::renderControl()
{
    RPlane::renderControl();

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
