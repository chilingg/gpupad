#include "RController.h"
#include "RDebug.h"

RController::RController(RController *parent):
    parent(nullptr),
    children()
{
    //setPatent(parent);
}

RController::~RController()
{
    setPatent(nullptr);
    close();
}

void RController::control()
{

}

void RController::setPatent(RController *parent)
{
    if(this->parent != nullptr)
    {
        this->parent->deleteChildren(this);
    }

    this->parent = parent;
    if(parent != nullptr)
        parent->addChildren(this);
}

void RController::addChildren(RController *child)
{
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        if(*it == child)
        {
            return;
        }
    }
    children.push_back(child);
}

void RController::deleteChildren(RController *child)
{
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        if(*it == child)
        {
            children.erase(it);
            return;
        }
    }
    printErro("Delete child do not exist!");
}

void RController::close()
{
    for(auto child : children)
    {
        child->close();
    }
    RControllers().swap(children);
    closeEvent();
}

void RController::update()
{
    for(auto child : children)
    {
        child->update();
    }
    paintEvent();
}

void RController::dispatcherResizeEvent(RResizeEvent *event)
{
    for(auto child : children)
    {
        child->dispatcherResizeEvent(event);
    }
    resizeEvent(event);
}

void RController::dispatcherInputEvent(RJoystickEvent *event, RController::Event name)
{
    for(auto child : children)
    {
        child->dispatcherInputEvent(event, name);
    }

    switch(name)
    {
    case JoystickEvent:
        joystickEvent(event);
        break;
    case JoystickConnectEvent:
        joystickConnectEvent(event);
        break;
    case JoystickDisconnectEvent:
        joystickDisconnectEvent(event);
        break;
    default:
        printErro("Error joystick event call");
    }
}

void RController::dispatcherInputEvent(RKeyEvent *event, RController::Event name)
{
    for(auto child : children)
    {
        child->dispatcherInputEvent(event, name);
    }

    switch(name)
    {
    case KeyPressEvent:
        keyPressEvent(event);
        break;
    case KeyReleaseEvent:
        keyReleaseEvent(event);
        break;
    default:
        printErro("Error key event call");
    }
}

void RController::dispatcherInputEvent(RMouseEvent *event, RController::Event name)
{
    for(auto child : children)
    {
        child->dispatcherInputEvent(event, name);
    }

    switch(name)
    {
    case MouseMoveEvent:
        mouseMoveEvent(event);
        break;
    case MousePressEvent:
        mousePressEvent(event);
        break;
    case MouseReleaseEvent:
        mouseReleaseEvent(event);
        break;
    default:
        printErro("Error mouse event call");
    }
}

void RController::dispatcherInputEvent(RWheelEvent *event, RController::Event name)
{
    for(auto child : children)
    {
        child->dispatcherInputEvent(event, name);
    }

    if(name == WheelEvent)
        wheelEvent(event);
    else
        printf("Error wheel event call");
}

void RController::paintEvent()
{

}

void RController::resizeEvent(RResizeEvent *event)
{

}

void RController::joystickConnectEvent(RJoystickEvent *)
{

}

void RController::joystickDisconnectEvent(RJoystickEvent *event)
{

}

void RController::joystickEvent(RJoystickEvent *)
{

}

void RController::keyPressEvent(RKeyEvent *)
{

}

void RController::keyReleaseEvent(RKeyEvent *)
{

}

void RController::mouseMoveEvent(RMouseEvent *)
{

}

void RController::mousePressEvent(RMouseEvent *)
{

}

void RController::mouseReleaseEvent(RMouseEvent *)
{

}

void RController::wheelEvent(RWheelEvent *)
{

}

void RController::closeEvent()
{

}
