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
    explicit RInitEvent(RController *looper):
        looper(looper) {}
    RController *looper;
};

class RCloseEvent
{
public:
    explicit RCloseEvent(RController *looper):
        looper(looper) {}
    RController *looper;
};

class REnteredTreeEvent
{
public:
    explicit REnteredTreeEvent(RController *spawner):
        spawner(spawner) {}
    RController *spawner;
};

class RExitedTreeEvent
{
public:
    explicit RExitedTreeEvent(RController *spawner):
        spawner(spawner) {}
    RController *spawner;
};

class RResizeEvent
{
public:
    RResizeEvent(int width, int height):
        width(width),
        height(height) {}
    const int width;
    const int height;
};

#endif // REVENT_H
