#ifndef RKEYEVENT_H
#define RKEYEVENT_H

#include <GLFW/glfw3.h>

class RKeyEvent
{
public:
    static const int No_Modifier = 0;
    static const int Shift_Modifier = GLFW_MOD_SHIFT;
    static const int Control_Modifier = GLFW_MOD_CONTROL;
    static const int Alt_Modifier = GLFW_MOD_ALT;
    static const int Super_Modifier = GLFW_MOD_SUPER;

    RKeyEvent(int key, int modifiers);
    int key() const;
    int modifiers() const;

private:
    const int KEY;
    const int MODIFIERS;
};

inline RKeyEvent::RKeyEvent(int key, int modifiers):
    KEY(key),
    MODIFIERS(modifiers)
{

}

inline int RKeyEvent::key() const
{
    return KEY;
}

inline int RKeyEvent::modifiers() const
{
    return MODIFIERS;
}

#endif // RKEYEVENT_H
