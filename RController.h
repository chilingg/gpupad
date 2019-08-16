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
    RController(RController *parent = nullptr);
    virtual ~RController();

    void setPatent(RController *parent);
    void addChildren(RController *child);
    void deleteChildren(RController *child);

protected:
    virtual void JoystickPresentEvent(RJoystickEvent *event);
    virtual void JoystickEvent(RJoystickEvent *event);
    virtual void keyPressEvent(RKeyEvent *event);
    virtual void keyReleaseEvent(RKeyEvent *event);
    virtual void mouseMoveEvent(RMouseEvent *event);
    virtual void mousePressEvent(RMouseEvent *event);
    virtual void mouseReleaseEvent(RMouseEvent *event);
    virtual void wheelEvent(RWheelEvent *event);

    RController *parent;
    RControllers children;
};

#endif // RCONTROLLER_H
