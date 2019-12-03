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

    plane_->rotateX(static_cast<float>(glfwGetTime()*2));
    plane_->RenderLineBox(0, width_, 0, height_);
    plane_->render();

    textPlane_.RenderLineBox(0, width_, 0, height_);
    textPlane_.render();

    FPS();

    sprite_.render();
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

    plane_.reset(new RPlane(80, 80, "text-plan", RPoint(480, 270, 0)));
    RImage img(":/texture/Robot_idle.png", "test-img", true);
    plane_->setTexture(img);
    plane_->setSizeMode(RPlane::Contain);
    //plane->flipV();
    plane_->setMargin(18);
    plane_->setPadding(10);
    plane_->setAlignment(RPlane::Align_Right, RPlane::Align_Top);
    RFont font(":/fonts/SourceHanSerifSC_EL-M/SourceHanSerifSC-Regular.otf", "testF");
    //font.loadFont("/home/carper/Code/Redopera/Redopera/Resource/fonts/SourceHanSerifSC_EL-M/SourceHanSerifSC-Regular.otf", 24);
    //auto glyph = font.getFontGlyph(L'j');
    //RDebug() << "bearingx:" << glyph.bearingX << " bearingy:" << glyph.bearingY
             //<< " w:" << glyph.width << " h:" << glyph.height << " advance:" << glyph.advance;
    //font.setFontSize(8);
    //font.getFontGlyph(L'A');
    //font.setAsDefualtFont(2);
    //auto data = font.getFontGlyph(L'大');
    //font.clearFontDataCache();
    //font.freeFont();

    textPlane_.setFont(font);
    textPlane_.setPosition(600, 200);
    textPlane_.setSize(200, 300);
    textPlane_.setFontSize(12);
    textPlane_.setMargin(10);
    textPlane_.setPadding(5);
    textPlane_.setWordSpacing(1.2f);
    textPlane_.setlineSpacing(1.5f);
    textPlane_.setAlignment(RPlane::Align_Right, RPlane::Align_Bottom);
    //textPlane_.verticalTypeset();
    textPlane_.setFontColor(240, 240, 255);
    //textPlane.setTexts(L"Aajfgi-_^——……读期间");
    //textPlane.setTexts(L"一二三四五六七八九");
    textPlane_.setTexts(L"#version 430 core\n"
                       "layout(points) in;\n"
                       "layout(line_strip, max_vertices = 5) out;\n"
                       "in vec2 size[1];\n"
                       "in vec3 color[1];\n"
                       "out vec3 lineColor;\n"
                       "void main(void)\n"
                       "{\n"
                           "lineColor = color[0];\n"
                           "vec4 position = gl_in[0].gl_Position;\n"
                       "\n"
                           "gl_Position = position;\n"
                           "EmitVertex();\n"
                           "gl_Position = position + vec4(0.0, size[0].y, 0.0, 0.0);\n"
                           "EmitVertex();\n"
                           "gl_Position = position + vec4(size[0], 0.0, 0.0);\n"
                           "EmitVertex();\n"
                           "gl_Position = position + vec4(size[0].x, 0.0, 0.0, 0.0);\n"
                           "EmitVertex();\n"
                           "gl_Position = position;\n"
                           "EmitVertex();\n"
                       "\n"
                           "EndPrimitive();\n"
                       "}\n");
    //textPlane_.setTexts(L"FreeType库是一个完全免费（开源）的、高质量的且可移植的字体引擎，"
                       //"它提供统一的接口来访问多种字体格式文件，包括TrueType, "
                       //"OpenType, Type1, CID, CFF, Windows FON/FNT, X11 PCF等。");

    fpsPlane_.rename("FPS-Plane");
    fpsPlane_.setWordSpacing(1.0f);
    fpsPlane_.setPosition(0, height_ - 50);
    fpsPlane_.setSize(100, 50);
    fpsPlane_.setFontSize(12);
    fpsPlane_.setFontColor(240, 50, 0);
    fpsPlane_.setPadding(10);
    fpsPlane_.setTexts(L"FPS:0");

    sprite_.rename("Test-Sprite");
    sprite_.setPosition(400, 300);
    RTexture f1, f2, f3, f4;
    f1.generate(":/texture/Robot_attacked.png");
    f2.generate(":/texture/Robot_idle.png");
    f3.generate(":/texture/Robot_injured.png");
    f4.generate(":/texture/Robot_normal.png");
    sprite_.addFrame(f1);
    sprite_.addFrame(f2);
    sprite_.addFrame(f3);
    sprite_.addFrame(f4);
    sprite_.setInterval(20);
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

void TestCtr::FPS()
{
    static int count = 0;
    ++count;
    if(timer_.elapsed() > 1.0)
    {
        fpsPlane_.setTexts(L"FPS:" + std::to_wstring(count));
        timer_.start();
        count = 0;
    }
    //fpsPlane_.RenderLineBox(0, width_, 0, height_);
    fpsPlane_.render();
}
