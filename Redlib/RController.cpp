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

void RController::initialization()
{
    for(auto child : children)
    {
        child->initialization();
    }
    initEvent();
}

void RController::dispatcherResizeEvent(RResizeEvent *event)
{
    for(auto child : children)
    {
        child->dispatcherResizeEvent(event);
    }
    resizeEvent(event);
}

void RController::dispatcherjoystickEvent(RJoystickEvent *event, Event name)
{
    for(auto child : children)
    {
        child->dispatcherjoystickEvent(event, name);
    }


    switch(name)
    {
    case JoystickPresentEvent:
        joystickPresentEvent(event);
        break;
    case JoystickInput:
        joystickInputEvent(event);
        break;
    default:
        printErro("Error jostick event call");
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

void RController::initEvent()
{

}

void RController::paintEvent()
{

}

void RController::resizeEvent(RResizeEvent *event)
{

}

void RController::joystickPresentEvent(RJoystickEvent *)
{

}

void RController::joystickInputEvent(RJoystickEvent *event)
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
