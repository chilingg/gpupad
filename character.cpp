#include "character.h"

Character::Character(int widht, int height):
    RObject(widht, height),
    _states(normal)
{

}

void Character::renderControl(RShaderProgram *shader)
{
    if(_states == jumped)
        color = { 1.0f, 0.0f, 0.0f, 1.0f};
    else
        color = { 0.5f, 0.5f, 0.5f, 1.0f};

    RObject::renderControl(shader);
}
