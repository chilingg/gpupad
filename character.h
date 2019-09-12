#ifndef CHARACTER_H
#define CHARACTER_H

#include <RSprite.h>

class Character : public RSprite
{
public:
    enum States{ quiet, moved, injured, attacked };

    Character(int widht, int height);
    States state();
    void setState(States s);

protected:
    void renderControl(RShaderProgram *shader) override;

private:
    using frameSequence = std::map<States, std::vector<std::string>>;

    States _states;
    States oldStates;
    frameSequence frames;
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
