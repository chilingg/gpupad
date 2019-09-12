#include "RSprite.h"

RSprite::RSprite(int width, int height):
    RTexObject(width, height),
    _interval(20),
    delta(0),
    texIndex(0)
{

}

bool RSprite::setCurrentTextureArray(std::vector<std::string> sequence, int interval)
{
    for(const auto &texName : sequence)
    {
        auto it = textures.find(texName);
        if(it == textures.end())
            return false;
    }

    using std::swap;
    swap(currentTexAry, sequence);
    currentTex = currentTexAry.front();
    setInterval(interval);

    return true;
}

void RSprite::renderControl(RShaderProgram *shader)
{
    RObject::renderControl(shader);

    if(delta > 0)
    {
        --delta;
    }
    else {
        delta = _interval;
        if(!textures.empty())
            texIndex = (texIndex + 1) % currentTexAry.size();
        else
            texIndex = 0;
    }

    textures[currentTexAry[texIndex]].bind();
}
