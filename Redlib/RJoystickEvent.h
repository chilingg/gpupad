#ifndef RJOYSTICKEVENT_H
#define RJOYSTICKEVENT_H

#include <RJoystick.h>

class RJoystickEvent
{
public:
    RJoystickEvent(int jid);

    int jid() const;
    bool isConnected() const;
    bool isDisconnected() const;
    int button() const;
    int buttonValue() const;
    int axis() const;
    float axisValue() const;

    void setButton(unsigned button, bool value);
    void setAxis(unsigned axis, float value);

private:
    const int _jid;
    bool _valid;
    int _button;
    bool _buttonValue;
    int _axis;
    float _axisValue;
};

inline RJoystickEvent::RJoystickEvent(int jid):
    _jid(jid),
    _valid(glfwJoystickIsGamepad(jid)),
    _button(-1),
    _buttonValue(false),
    _axis(-1),
    _axisValue(0.0f)
{
}

inline int RJoystickEvent::jid() const
{
    return _jid;
}

inline bool RJoystickEvent::isConnected() const
{
    return _valid;
}

inline bool RJoystickEvent::isDisconnected() const
{
    return !_valid;
}

inline int RJoystickEvent::button() const
{
    return _button;
}

inline int RJoystickEvent::buttonValue() const
{
    return _buttonValue;
}

inline int RJoystickEvent::axis() const
{
    return _axis;
}

inline float RJoystickEvent::axisValue() const
{
    return _axisValue;
}

inline void RJoystickEvent::setButton(unsigned button, bool value)
{
    _button = static_cast<int>(button);
    _buttonValue = value;
}

inline void RJoystickEvent::setAxis(unsigned axis, float value)
{
    _axis = static_cast<int>(axis);
    _axisValue = value;
}

#endif // RJOYSTICKEVENT_H
