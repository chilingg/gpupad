#ifndef RMOUSEEVENT_H
#define RMOUSEEVENT_H

#include <glm/vec2.hpp>

class RMouseEvent
{
public:
    enum MouseButton{
        Mouse_Button_Left,
        Mouse_Button_Right,
        Mouse_Button_Middle,
        Mouse_Button_None
    };
    RMouseEvent(int x, int y, int button = Mouse_Button_None, int mods = 0);
    int x() const;
    int y() const;
    int button() const;
    int modifiers() const;

private:
    const int X;
    const int Y;
    const int BUTTON;
    const int MODIFIERS;
};

inline RMouseEvent::RMouseEvent(int x, int y, int button, int mods):
    X(x),
    Y(y),
    BUTTON(button),
    MODIFIERS(mods)
{

}

inline int RMouseEvent::x() const
{
    return X;
}

inline int RMouseEvent::y() const
{
    return Y;
}

inline int RMouseEvent::button() const
{
    return BUTTON;
}

inline int RMouseEvent::modifiers() const
{
    return MODIFIERS;
}

#endif // RMOUSEEVENT_H
