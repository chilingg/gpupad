#ifndef RJOYSTICK_H
#define RJOYSTICK_H

#include <GLFW/glfw3.h>

class RJoystick
{
public:
    enum Buttons {
        GAMEPAD_BUTTON_A = GLFW_GAMEPAD_BUTTON_A,
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
        GAMEPAD_BUTTON_LAST = GAMEPAD_BUTTON_DPAD_LEFT
    };
    enum Axes {
        GAMEPAD_AXIS_LEFT_X,
        GAMEPAD_AXIS_LEFT_Y,
        GAMEPAD_AXIS_RIGHT_X,
        GAMEPAD_AXIS_RIGHT_Y,
        GAMEPAD_AXIS_LEFT_TRIGGER,
        GAMEPAD_AXIS_RIGHT_TRIGGER,
        GAMEPAD_AXIS_LAST = GAMEPAD_AXIS_RIGHT_TRIGGER
    };

    RJoystick();
    RJoystick(int jid);
    bool button(Buttons btn) const;
    bool button(unsigned btn) const;
    float axes(Axes axis) const;
    float axes(unsigned axis) const;
    int jid() const;
    bool setJid(int jid);
    bool isConnected() const;
    bool isDisconnected() const;
    void setInvalid();
    bool update();

private:
    int _jid;
    GLFWgamepadstate _status;
    bool _valid;
};

inline RJoystick::RJoystick()
{
    _valid = false;
}

inline RJoystick::RJoystick(int jid):
    _jid(jid)
{
    _valid = glfwGetGamepadState(jid, &_status);
}

inline bool RJoystick::button(RJoystick::Buttons btn) const
{
    //glfwGetGamepadState(_jid, &_status);
    return _status.buttons[btn];
}

inline bool RJoystick::button(unsigned btn) const
{
    return _status.buttons[btn];
}

inline float RJoystick::axes(RJoystick::Axes axis) const
{
    return _status.axes[axis];
}

inline float RJoystick::axes(unsigned axis) const
{
    return _status.axes[axis];
}

inline int RJoystick::jid() const
{
    return _jid;
}

inline bool RJoystick::setJid(int jid)
{
    _jid = jid;
    _valid = glfwGetGamepadState(jid, &_status);
    return _valid;
}

inline bool RJoystick::isConnected() const
{
    return _valid;
}

inline bool RJoystick::isDisconnected() const
{
    return !_valid;
}

inline void RJoystick::setInvalid()
{
    _valid = false;
}

inline bool RJoystick::update()
{
    return _valid = glfwGetGamepadState(_jid, &_status);
}

#endif // RJOYSTICK_H
