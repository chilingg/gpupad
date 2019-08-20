#ifndef RJOYSTICKEVENT_H
#define RJOYSTICKEVENT_H

#include <GLFW/glfw3.h>

class RJoystickEvent
{
    enum Buttons {
        GAMEPAD_BUTTON_A,
        GAMEPAD_BUTTON_B,
        GAMEPAD_BUTTON_X,
        GAMEPAD_BUTTON_Y,
        GAMEPAD_BUTTON_LEFT_BUMPER,
        GAMEPAD_BUTTON_RIGHT_BUMPER,
        GAMEPAD_BUTTON_BACK,
        GAMEPAD_BUTTON_START,
        GAMEPAD_BUTTON_GUIDE,
        GAMEPAD_BUTTON_LEFT_THUMB,
        GAMEPAD_BUTTON_RIGHT_THUMB,
        GAMEPAD_BUTTON_DPAD_UP,
        GAMEPAD_BUTTON_DPAD_RIGHT,
        GAMEPAD_BUTTON_DPAD_DOWN,
        GAMEPAD_BUTTON_DPAD_LEFT,
        GAMEPAD_BUTTON_LAST
    };
    enum Axes {
        GAMEPAD_AXIS_LEFT_X,
        GAMEPAD_AXIS_LEFT_Y,
        GAMEPAD_AXIS_RIGHT_X,
        GAMEPAD_AXIS_RIGHT_Y,
        GAMEPAD_AXIS_LEFT_TRIGGER,
        GAMEPAD_AXIS_RIGHT_TRIGGER
    };

public:
    RJoystickEvent(int jid);
    bool button(Buttons btn) const;
    float axes(Axes axis) const;
    int jid() const;

private:
    const int JID;
    GLFWgamepadstate status;
    bool valid;
};

inline RJoystickEvent::RJoystickEvent(int jid):
    JID(jid)
{
    valid = glfwGetGamepadState(jid, &status);
}

inline bool RJoystickEvent::button(RJoystickEvent::Buttons btn) const
{
    return status.buttons[btn];
}

inline float RJoystickEvent::axes(RJoystickEvent::Axes axis) const
{
    return status.axes[axis];
}

inline int RJoystickEvent::jid() const
{
    return JID;
}

#endif // RJOYSTICKEVENT_H
