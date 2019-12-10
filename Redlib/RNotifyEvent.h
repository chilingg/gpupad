#ifndef REVENT_H
#define REVENT_H

#include <string>
#include "RInputEvent.h"

class RController;

//通知事件 ************************

class RjoystickPresentEvent
{
public:
    RjoystickPresentEvent(RInputEvent::JoystickID jid, bool connected):
        jid(jid),
        connected(connected) {}
    RInputEvent::JoystickID jid;
    const bool connected;
};

class RInitEvent
{
public:
    explicit RInitEvent(RController *sender):
        sender(sender) {}
    RController *sender;
};

class RCloseEvent
{
public:
    explicit RCloseEvent(RController *sender):
        sender(sender) {}
    RController *sender;
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

class RResizeEvent
{
public:
    RResizeEvent(RController *sender, int width, int height):
        sender(sender),
        width(width),
        height(height) {}
    RController *sender;
    const int width;
    const int height;
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

#endif // REVENT_H
