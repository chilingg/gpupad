#include "character.h"
#include "constant.h"

Character::Character(int widht, int height):
    RTexObject(widht, height),
    _states(quiet)
{
    addTexture("Quiet", RE_PATH+"texture/Robot_idle.png");
    addTexture("Move", RE_PATH+"texture/Robot_normal.png");
    addTexture("Attacked", RE_PATH+"texture/Robot_attacked.png");
    addTexture("Injured", RE_PATH+"texture/Robot_injured.png");
}

void Character::renderControl(RShaderProgram *shader)
{
    if(_states == moved)
        setCurrentTexture("Move");
    else
        setCurrentTexture("Quiet");

    RTexObject::renderControl(shader);
}
