#ifndef RCONTROLLER_H
#define RCONTROLLER_H

#include "RJoystickEvent.h"
#include "RKeyEvent.h"
#include "RMouseEvent.h"
#include "RWheelEvent.h"
#include "RResizeEvent.h"
#include <list>

class RController
{
    using RControllers = std::list<RController*>;

public:
    enum Event {
        KeyPressEvent,
        KeyReleaseEvent,
        MouseMoveEvent,
        MousePressEvent,
        MouseReleaseEvent,
        WheelEvent,
        JoystickPresentEvent,
        JoystickInput
    };

    RController(RController *parent = nullptr);
    virtual ~RController();
    virtual void control() = 0;

    void setPatent(RController *parent);
    void addChildren(RController *child);
    void deleteChildren(RController *child);

    void close();
    void update();
    void initialization();
    void dispatcherResizeEvent(RResizeEvent *event);
    void dispatcherjoystickEvent(RJoystickEvent *event, Event name);

    void dispatcherInputEvent(RKeyEvent *event, Event name);
    void dispatcherInputEvent(RMouseEvent *event, Event name);
    void dispatcherInputEvent(RWheelEvent *event, Event name);

protected:
    virtual void initEvent();
    virtual void paintEvent();
    virtual void resizeEvent(RResizeEvent *event);
    virtual void joystickPresentEvent(RJoystickEvent *event);
    virtual void joystickInputEvent(RJoystickEvent *event);
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
