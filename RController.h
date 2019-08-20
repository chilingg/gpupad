#ifndef RCONTROLLER_H
#define RCONTROLLER_H

#include "RJoystickEvent.h"
#include "RKeyEvent.h"
#include "RMouseEvent.h"
#include "RWheelEvent.h"
#include <list>

class RController
{
    using RControllers = std::list<RController*>;

public:
    enum Event {
        PaintEvent,
        JoystickEvent,
        JoystickConnectEvent,
        JoystickDisconnectEvent,
        KeyPressEvent,
        KeyReleaseEvent,
        MouseMoveEvent,
        MousePressEvent,
        MouseReleaseEvent,
        WheelEvent
    };

    RController(RController *parent = nullptr);
    virtual ~RController();

    void setPatent(RController *parent);
    void addChildren(RController *child);
    void deleteChildren(RController *child);

    void close();
    void update();
    void resize(int width, int height);

    void dispatcherInputEvent(RJoystickEvent *event, Event name);
    void dispatcherInputEvent(RKeyEvent *event, Event name);
    void dispatcherInputEvent(RMouseEvent *event, Event name);
    void dispatcherInputEvent(RWheelEvent *event, Event name);

protected:
    virtual void paintEvent();
    virtual void resizeEvent(int width, int height);
    virtual void joystickConnectEvent(RJoystickEvent *event);
    virtual void joystickDisconnectEvent(RJoystickEvent *event);
    virtual void joystickEvent(RJoystickEvent *event);
    virtual void keyPressEvent(RKeyEvent *event);
    virtual void keyReleaseEvent(RKeyEvent *event);
    virtual void mouseMoveEvent(RMouseEvent *event);
    virtual void mousePressEvent(RMouseEvent *event);
    virtual void mouseReleaseEvent(RMouseEvent *event);
    virtual void wheelEvent(RWheelEvent *event);
    virtual void closeEvent();

    RController *parent;
    RControllers children;
};

#endif // RCONTROLLER_H
