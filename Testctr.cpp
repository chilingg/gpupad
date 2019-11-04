#include "Testctr.h"

#include <RResource/RResource.h>
#include <RDebug.h>

TestCtr::TestCtr(const std::string &name, RController *parent):
    RController(name, parent)
{

}

void TestCtr::control()
{

}

void TestCtr::inputEvent(const RInputEvent *event)
{
    if(event->checkButton(RInputEvent::KEY_ESCAPE) == RInputEvent::PRESS)
        closed.emit();
}

void TestCtr::initEvent(RInitEvent *event)
{
    RDebug() << "Initialization " << event->looper->getName() << " in " << getPathName();
    closed.connect(event->looper, &RController::inactive);
}

void TestCtr::joystickPresentEvent(RjoystickPresentEvent *event)
{
    if(event->connected && !gamepad_.connected)
    {
        gamepad_.jid = event->jid;
        gamepad_.connected = true;
    }
    else if(!event->connected && event->jid == gamepad_.jid)
        gamepad_.connected = false;
    RDebug() << "JID: " << gamepad_.jid << " Present: " << gamepad_.connected;
}

void TestCtr::resizeEvent(RResizeEvent *event)
{

}

void TestCtr::exitedTreeEvent(RExitedTreeEvent *event)
{
    RDebug() << "Exited tree with the " << event->treeNodeName << " of the " << getPathName();
}

void TestCtr::enteredTreeEvent(REnteredTreeEvent *event)
{
    RDebug() << "Entered tree with the " << event->treeNodeName << " of the " << getPathName();
}

void TestCtr::closeEvent(RCloseEvent *event)
{
    RDebug() << "Close " << event->looper->getName() << " in " << getPathName();
    closed.disconnect(event->looper);
}
