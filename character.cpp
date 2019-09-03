#include "character.h"

Character::Character(int widht, int height, const char *path):
    RTexObject(widht, height, path),
    _states(normal)
{

}

Character::Character(int widht, int height, const std::string &path):
    RTexObject(widht, height, path.c_str())
{

}

void Character::renderControl(RShaderProgram *shader)
{
    RTexObject::renderControl(shader);
    if(_states == jumped)
        color = { 1.0f, 0.0f, 0.0f, 1.0f};
    else
        color = { 1.0f, 1.0f, 1.0f, 1.0f};

    RObject::renderControl(shader);
    RTexObject::renderControl(shader);
}
