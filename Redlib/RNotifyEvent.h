#ifndef REVENT_H
#define REVENT_H

#include <string>
#include "RInputModule.h"
#include "RDebug.h"

class RController;

//通知事件 ************************

class RjoystickPresentEvent
{
public:
    RjoystickPresentEvent(RInputModule::JoystickID jid, bool connected):
        jid(jid),
        connected(connected) {}
    RInputModule::JoystickID jid;
    const bool connected;
};

class RStartEvent
{
public:
    explicit RStartEvent(RController *sender):
        sender(sender) {}
    RController *sender;
};

class RFinishEvent
{
public:
    explicit RFinishEvent(RController *sender):
        sender(sender) {}
    RController *sender;
};

class RCloseEvent
{
public:
    explicit RCloseEvent(RController *sender):
        sender(sender) {}
    RController *sender;
    bool stop = false;
};

class REnteredTreeEvent
{
public:
    explicit REnteredTreeEvent(RController *sender):
        sender(sender) {}
    RController *sender;
};

class RExitedTreeEvent
{
public:
    explicit RExitedTreeEvent(RController *sender):
        sender(sender) {}
    RController *sender;
};

class RScrollEvent
{
public:
    RScrollEvent(RController *sender, int direct, int x, int y):
        sender(sender),
        direct(direct),
        x(x),
        y(y) {}
    RController *sender;
    const int direct;
    const int x;
    const int y;
};

class RInputEvent
{
public:
    RInputEvent(RController *sender):
        sender(sender) {}

    RInputModule::ButtonAction status(RInputModule::Keys key) {
        return RInputModule::instance().keyInputs_[key].action;
    }
    RInputModule::ButtonAction status(RInputModule::MouseButtons btn) {
        return RInputModule::instance().mouseInputs_[btn].action;
    }
    RInputModule::ButtonAction status(RInputModule::GamepadButtons btn, unsigned p = 0) {
        assert(p < RInputModule::instance().gamepadInputs_.size());
        return RInputModule::toButtonAction(RInputModule::instance().gamepadInputs_[p].status.buttons[btn]);
    }
    float status(RInputModule::GamepadAxes axis, unsigned p = 0) {
        assert(p < RInputModule::instance().gamepadInputs_.size());
        return RInputModule::instance().gamepadInputs_[p].status.axes[axis];
    }

    bool press(RInputModule::Keys key) {
        return RInputModule::instance().keyInputs_[key].action == RInputModule::PRESS
                && RInputModule::instance().keyInputs_[key].action != RInputModule::instance().keyInputs_[key].preAction;
    }
    bool press(RInputModule::MouseButtons btn) {
        return RInputModule::instance().mouseInputs_[btn].action == RInputModule::PRESS
                && RInputModule::instance().mouseInputs_[btn].action != RInputModule::instance().mouseInputs_[btn].preAction;
    }
    bool press(RInputModule::GamepadButtons btn, unsigned p = 0) {
        assert(p < RInputModule::instance().gamepadInputs_.size());
        return RInputModule::instance().gamepadInputs_[p].status.buttons[btn] == RInputModule::PRESS
                && RInputModule::instance().gamepadInputs_[p].status.buttons[btn] != RInputModule::instance().gamepadInputs_[p].preButtons[btn];
    }

    bool release(RInputModule::Keys key) {
        return RInputModule::instance().keyInputs_[key].action == RInputModule::RELEASE
                && RInputModule::instance().keyInputs_[key].action != RInputModule::instance().keyInputs_[key].preAction;
    }
    bool release(RInputModule::MouseButtons btn) {
        return RInputModule::instance().mouseInputs_[btn].action == RInputModule::RELEASE
                && RInputModule::instance().mouseInputs_[btn].action != RInputModule::instance().mouseInputs_[btn].preAction;
    }
    bool release(RInputModule::GamepadButtons btn, unsigned p = 0) {
        assert(p < RInputModule::instance().gamepadInputs_.size());
        return RInputModule::instance().gamepadInputs_[p].status.buttons[btn] == RInputModule::RELEASE
                && RInputModule::instance().gamepadInputs_[p].status.buttons[btn] != RInputModule::instance().gamepadInputs_[p].preButtons[btn];
    }

    RPoint2 cursorPos() { return RInputModule::instance().cursorPos_; }

    RController *sender;
    void *userData;
};

#endif // REVENT_H
