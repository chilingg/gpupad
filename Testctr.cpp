#include "Testctr.h"

#include <RDebug.h>
#include <RWindowCtrl.h>

TestCtr::TestCtr(const std::string &name, RController *parent):
    RController(name, parent)
{
    if(!gamepads.empty())
    {
        gamepad_.jid = *gamepads.begin();
        gamepad_.connected = true;
    }
}

void TestCtr::control()
{

}

void TestCtr::scrollNotify(double v)
{
    RDebug() << v;
}

void TestCtr::inputEvent(const RInputEvent *event)
{
    if(event->checkButton(RInputEvent::KEY_ESCAPE) == RInputEvent::PRESS)
        closed.emit();
    if(event->checkButton(RInputEvent::KEY_F11) != fullScreen_)
    {
        if(fullScreen_ == RInputEvent::PRESS)
            if(RWindowCtrl *window = dynamic_cast<RWindowCtrl*>(getParent()))
                window->setFullScreenWindow(fullScreenB_ = !fullScreenB_);
        fullScreen_ = event->checkButton(RInputEvent::KEY_F11);
    }
    if(event->checkMouseButton(RInputEvent::Mouse_Button_Right).isValid())
        RDebug() << event->checkMouseButton(RInputEvent::Mouse_None);
    if(gamepad_.connected)
    {
        if(event->checkButton(gamepad_.jid, RInputEvent::GAMEPAD_BUTTON_A) == RInputEvent::PRESS)
            RDebug() << "Gamepad button A";
        if(event->checkGamepadAxis(gamepad_.jid, RInputEvent::GAMEPAD_AXIS_LEFT_X) >= 0.5f)
            RDebug() << event->checkGamepadAxis(gamepad_.jid, RInputEvent::GAMEPAD_AXIS_LEFT_X);
        if(event->checkGamepadAxis(gamepad_.jid, RInputEvent::GAMEPAD_AXIS_LEFT_TRIGGER) > 0.0f)
            RDebug() << event->checkGamepadAxis(gamepad_.jid, RInputEvent::GAMEPAD_AXIS_LEFT_TRIGGER);
    }
}

void TestCtr::initEvent(RInitEvent *event)
{
    RDebug() << "Initialization " << event->looper->getName() << " in " << getPathName();
    closed.connect(event->looper, &RController::inactive);
    if(RWindowCtrl *window = dynamic_cast<RWindowCtrl*>(event->looper))
        window->scrolled.connect(this, &TestCtr::scrollNotify);
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
    RDebug() << event->width << event->height << "Resize event";
}

void TestCtr::exitedTreeEvent(RExitedTreeEvent *event)
{
    RDebug() << "Exited tree with the " << event->spawner->getName() << " of the " << getPathName();
}

void TestCtr::enteredTreeEvent(REnteredTreeEvent *event)
{
    RDebug() << "Entered tree with the " << event->spawner->getName() << " of the " << getPathName();
}

void TestCtr::closeEvent(RCloseEvent *event)
{
    RDebug() << "Close " << event->looper->getName() << " in " << getPathName();
    closed.disconnect(event->looper);
}
