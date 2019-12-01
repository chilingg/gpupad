#include "Testctr.h"

#include <RDebug.h>
#include <RWindowCtrl.h>
#include <RResource/RImage.h>
#include <RResource/RTexture.h>
#include <RResource/RShader.h>
#include <RResource/RShaderProgram.h>
#include <RResource/RFont.h>
#include <RColor.h>

#include <glm/matrix.hpp>

TestCtr::TestCtr(const std::string &name, RController *parent):
    RController(name, parent),
    closed()
{
    if(!gamepads.empty())
    {
        gamepad_.jid = *gamepads.begin();
        gamepad_.connected = true;
    }
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

    //plane->rotateX(static_cast<float>(glfwGetTime()*2));
    plane->RenderLineBox(0, width_, 0, height_);
    plane->render();

    textPlane.RenderLineBox(0, width_, 0, height_);
    textPlane.render();
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
    {
        //connect(window, &RWindowCtrl::scrolled, this, &TestCtr::scrollNotify);
        window->scrolled.connect(this, &TestCtr::scrollNotify);
        width_ = window->width();
        height_ = window->height();
    }

    plane.reset(new RPlane(80, 80, "text-plan", RPoint(480, 270, 0)));
    RImage img(":/texture/Robot_idle.png", "test-img", true);
    plane->setTexture(img);
    plane->setSizeMode(RPlane::Contain);
    //plane->flipV();
    plane->setMargin(18);
    plane->setPadding(10);
    plane->setAlignment(RPlane::Align_Right, RPlane::Align_Top);
    RFont font;
    font.loadFont("/home/carper/Code/Redopera/Redopera/Resource/fonts/SourceHanSerifSC_EL-M/SourceHanSerifSC-Regular.otf", 24);
    auto glyph = font.getFontGlyph(L'j');
    //RDebug() << "bearingx:" << glyph.bearingX << " bearingy:" << glyph.bearingY
             //<< " w:" << glyph.width << " h:" << glyph.height << " advance:" << glyph.advance;
    //font.setFontSize(8);
    //font.getFontGlyph(L'A');
    //font.setAsDefualtFont(2);
    //auto data = font.getFontGlyph(L'大');
    //font.clearFontDataCache();
    //font.freeFont();

    textPlane.setFont(font);
    textPlane.setPosition(10, 200);
    textPlane.setSize(200, 300);
    textPlane.setFontSize(12);
    //textPlane.setTexts(L"Aajfgi-_^——……读期间");
    //textPlane.setTexts(L"一二三四五六七八九");
    textPlane.setWordSpacing(1.1f);
    textPlane.setTexts(L"https://www.cnblogs.com/PrayG/p/5749832.html\n\
                       用ios::sync_with_stdio(false)有什么坏处？ - 知乎\
                       \
                       2017-7-5 · 好久没写c艹，强答一下 背景姿势： 系统默认standard stream应该都是同步的，你设置sync_with_stdio(false)，其实应该是让C风格的stream和C艹风格的stream变成async且分用不同buffer。\
                       https://www.zhihu.com/question/62041072\
                       LeetCode 用 C++ 刷题时是否应该使用 I/O 优化代码？ - 知乎\
                           2019-2-6\
                       leetcode里这段强行加快运行速度的C++代码是什么意思？ - 知乎\
                           2018-6-23\
                       c++中ios::sync_with_stdio(false)后可以使用freopen()吗? - 知乎\
                           2018-2-15\
                       为什么 C 语言的输入输出函数比 C++ 的输入输出流要快？ - 知乎\
                           2012-8-30\
                       查看更多结果\
                       std::ios_base::sync_with_stdio - cppreference.com\
                       \
                       2017-11-23 · 本页面最后修改于2017年11月23日 (星期四) 22:42。 此页面已被浏览过5,");
    textPlane.setTexts(L"ml\n\
                       用ios::sync_with乎 ");
    //textPlane.setTexts(L"https://www.cnblogs.com/PrayG/p/5749832.html");
    textPlane.setMargin(10);
    textPlane.setPadding(5);
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
    width_ = event->width; height_ = event->height;
    //RPlane::setPlaneDefaultViewpro(0, event->width, 0, event->height);
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
