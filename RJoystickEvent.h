#ifndef RJOYSTICKEVENT_H
#define RJOYSTICKEVENT_H

#include <set>
class RJoystickEvent
{
    using Joysticks = std::set<int>;
public:
    RJoystickEvent(Joysticks joys);
    int key() const;
    int modifiers() const;

private:
    const Joysticks  joys;
};
#endif // RJOYSTICKEVENT_H
