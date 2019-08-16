#include "RController.h"
#include "RDebug.h"

RController::RController(RController *parent):
    parent(nullptr),
    children()
{
    setPatent(parent);
}

RController::~RController()
{
    setPatent(nullptr);
    pass;
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

void RController::JoystickPresentEvent(RJoystickEvent *)
{

}

void RController::JoystickEvent(RJoystickEvent *)
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
