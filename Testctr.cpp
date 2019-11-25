#include "Testctr.h"

#include <RDebug.h>
#include <RWindowCtrl.h>
#include <RResource/RImage.h>
#include <RResource/RTexture.h>
#include <RResource/RShader.h>
#include <RResource/RShaderProgram.h>
#include <RColor.h>

#include <glm/matrix.hpp>

TestCtr::TestCtr(const std::string &name, RController *parent):
    RController(name, parent),
    plant("text-plan", 32, 32, RPoint(20, 20))
{
    if(!gamepads.empty())
    {
        gamepad_.jid = *gamepads.begin();
        gamepad_.connected = true;
    }

    RImage img(":/texture/Robot_idle.png", "test-img");
    RTexture tex(img, "test-img");
    plant.setTexture(tex);
}

TestCtr::~TestCtr()
{
    if(debugWindow_)
        delete debugWindow_;
}

void TestCtr::control()
{
    allChildrenActive();
    if(debugWindow_ && debugWindow_->isShouldCloused())
    {
        RDebug() << "Delete a Debug Window";
        delete debugWindow_;
        debugWindow_ = nullptr;
    }

    plant.render();
}

void TestCtr::scrollNotify(double v)
{
    RDebug() << v;
}

void TestCtr::inputEvent(const RInputEvent *event)
{
    if(event->checkButton(RInputEvent::KEY_ESCAPE) == RInputEvent::PRESS)
        closed.emit();
    if(event->checkButton(RInputEvent::KEY_F11) != fullScreenBtn_)
    {
        if(fullScreenBtn_ == RInputEvent::PRESS)
            if(RWindowCtrl *window = dynamic_cast<RWindowCtrl*>(getParent()))
                window->setFullScreenWindow(fullScreen_ = !fullScreen_);
        fullScreenBtn_ = event->checkButton(RInputEvent::KEY_F11);
    }
    if(event->checkButton(RInputEvent::KEY_F12) != debugWindowBtn_)
    {
        if(debugWindowBtn_ == RInputEvent::PRESS)
        {
            if(!debugWindow_)
            {
                debugWindow_ = new RWindowCtrl("Debug", this);
                debugWindow_->setWindowTitle("Debug");
                //debugWindow_->setWindowDecrate(false);
                //debugWindow_->setWindowFloatOnTop(true);
                debugWindow_->showWindow();
            }
        }
        debugWindowBtn_ = event->checkButton(RInputEvent::KEY_F12);
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
    closed.connect(event->looper, &RController::breakLoop);
    if(RWindowCtrl *window = dynamic_cast<RWindowCtrl*>(event->looper))
        //connect(window, &RWindowCtrl::scrolled, this, &TestCtr::scrollNotify);
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
