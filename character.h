#ifndef CHARACTER_H
#define CHARACTER_H

#include <RTexObject.h>

class Character : public RTexObject
{
public:
    enum States{ quiet, moved, injured, attacked };

    Character(int widht, int height);
    States state();
    void setState(States s);

protected:
    void renderControl(RShaderProgram *shader) override;

private:
    States _states;
};

inline Character::States Character::state()
{
    return _states;
}

inline void Character::setState(Character::States s)
{
    _states = s;
}

#endif // CHARACTER_H
