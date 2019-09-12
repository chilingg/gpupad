#include "character.h"
#include "constant.h"

Character::Character(int widht, int height):
    RSprite(widht, height),
    _states(quiet),
    oldStates(quiet)
{
    addTexture("Quiet", RE_PATH+"texture/Robot_idle.png");
    addTexture("Move", RE_PATH+"texture/Robot_normal.png");
    addTexture("Attacked", RE_PATH+"texture/Robot_attacked.png");
    addTexture("Injured", RE_PATH+"texture/Robot_injured.png");

    frames[quiet] = {"Move", "Quiet"};
    frames[moved] = {"Move"};
    frames[injured] = {"Injured"};
    frames[attacked] = {"Attacked"};

    setCurrentTextureArray(frames[_states]);
}

void Character::renderControl(RShaderProgram *shader)
{
    if(_states != oldStates)
    {
        setCurrentTextureArray(frames[_states]);
        oldStates = _states;
    }

    RSprite::renderControl(shader);
}
