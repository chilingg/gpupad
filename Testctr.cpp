#include "Testctr.h"

#include <RDebug.h>
#include <RWindowCtrl.h>
#include <RResource/RImage.h>
#include <RResource/RTexture.h>
#include <RResource/RShader.h>
#include <RResource/RFont.h>
#include <RColor.h>

#include <glm/matrix.hpp>

TestCtr::TestCtr(const std::string &name, RController *parent):
    RController(name, parent),
    closed()
{
}

TestCtr::~TestCtr()
{
}

void TestCtr::control()
{
    allChildrenActive();
    bColor_.render();

    plane_->rotateX(static_cast<float>(glfwGetTime()*2));
    plane_->renderLineBox(0, 960, 0, 540);
    plane_->render();

    textPlane_.renderLineBox(0, width_, 0, height_);
    textPlane_.render();

    FPS();

    sprite_.render();
}

void TestCtr::inputEvent(RInputEvent *event)
{
    if(event->press(RInputModule::KEY_ESCAPE))
        closed.emit();

    if(event->press(RInputModule::KEY_F11))
    {
        if(RWindowCtrl *window = dynamic_cast<RWindowCtrl*>(getParent()))
            window->setFullScreenWindow(fullScreen_ = !fullScreen_);
    }

    if(event->press(RInputModule::KEY_F12))
    {
        RDebug() << getFreeTree()->getTreeNode("WindowCtrl/TestCtrl");
        if(!getFreeTree()->getTreeNode("ResourceWindow"))
        {
            reWindowThread_ = RThread([](){
                RResourceWindow reWindow;
                reWindow.showWindow();
                reWindow.exec();
            });
        }
    }

    if(event->press(RInputModule::KEY_P))
    {
        tick_.setStreamTime();
        if(!tick_.isRunning()) tick_.startStream();
    }
    if(event->press(RInputModule::KEY_O))
    {
        if(!bgm_.isRunning()) bgm_.repeatStream();
        else bgm_.abortStream();
    }
    if(event->status(RInputModule::KEY_KP_ADD) == RInputModule::PRESS)
    {
        if(bgmSetTimer_.elapsed() > 100)
        {
            bgm_.increaseVolume();
            RDebug() << "BGM Volume: " << bgm_.getVolume() * 100;
            bgmSetTimer_.start();
        }
    }
    if(event->status(RInputModule::KEY_KP_SUBTRACT) == RInputModule::PRESS)
    {
        if(bgmSetTimer_.elapsed() > 100)
        {
            bgm_.decreaseVolume();
            RDebug() << "BGM Volume: " << bgm_.getVolume() * 100;
            bgmSetTimer_.start();
        }
    }

    if(event->press(RInputModule::MOUSE_BUTTON_RIGHT))
        RDebug() << event->cursorPos();

    if(RInputModule::instance().gamepadCount())
    {
        if(event->press(RInputModule::GAMEPAD_BUTTON_A))
            RDebug() << "Gamepad button A:" << event->status(RInputModule::GAMEPAD_BUTTON_A);
        if(event->status(RInputModule::GAMEPAD_AXIS_LEFT_X) >= 0.5f)
            RDebug() << event->status(RInputModule::GAMEPAD_AXIS_LEFT_X);
        if(event->status(RInputModule::GAMEPAD_AXIS_LEFT_TRIGGER) > 0.0f)
            RDebug() << event->status(RInputModule::GAMEPAD_AXIS_LEFT_TRIGGER);
    }
}

void TestCtr::initEvent(RInitEvent *event)
{
    RDebug() << "Initialization " << event->sender->name() << " in " << getPathName();

    closed.connect(event->sender, &RController::breakLoop);

    if(RWindowCtrl *window = dynamic_cast<RWindowCtrl*>(event->sender))
    {
        //connect(window, &RWindowCtrl::scrolled, this, &TestCtr::scrollNotify);
        width_ = window->width();
        height_ = window->height();
    }

    RTexture::hintTexFilter(RTexture::Nearest);

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

    uiShaders_ = RShaderProgram::getStanderdShaderProgram();
    uiShaders_.use();
    uiShaders_.setViewprot("projection", 0, 960, 0, 540);
    uiShaders_.setCameraPos("view", 0, 0);

    textPlane_.setShaderProgram(uiShaders_, uiShaders_.getUniformLocation("model"));
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

    fpsPlane_.setShaderProgram(uiShaders_, uiShaders_.getUniformLocation("model"));
    fpsPlane_.rename("FPS-Plane");
    fpsPlane_.setWordSpacing(1.0f);
    fpsPlane_.setPosition(0, height_ - 50);
    fpsPlane_.setSize(100, 50);
    fpsPlane_.setFontSize(13);
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

    bColor_.setSize(width_, height_);
    bColor_.setColorTexture(28, 28, 28);
    bColor_.setPosition(0, 0, -127);

    bgm_.openStream(RMp3(":/music/bgm.mp3","Test-BGM"));
    tick_.openStream(RMp3(":/music/tick.mp3","Test-tick"));
}

void TestCtr::resizeEvent(RResizeEvent *event)
{
    width_ = event->width; height_ = event->height;
    RMatrix4 projection = RMath::ortho(0.0f, static_cast<float>(width_), 0.0f, static_cast<float>(height_));
    uiShaders_.use();
    uiShaders_.setUniformMatrix(uiShaders_.getUniformLocation("projection"), 4, RMath::value_ptr(projection));
    uiShaders_.nonuse();

    fpsPlane_.setOuterPositionY(event->height - fpsPlane_.outerHeight());
    bColor_.setSize(event->width, event->height);

    RDebug() << event->width << event->height << "Resize event";
}

void TestCtr::exitedTreeEvent(RExitedTreeEvent *event)
{
    RDebug() << "Exited tree with the " << event->sender->name() << " of the " << getPathName();
}

void TestCtr::enteredTreeEvent(REnteredTreeEvent *event)
{
    RDebug() << "Entered tree with the " << event->sender->name() << " of the " << getPathName();
}

void TestCtr::closeEvent(RCloseEvent *event)
{
    RDebug() << "Close " << event->sender->name() << " in " << getPathName();
    closed.disconnect(event->sender);
}

void TestCtr::FPS()
{
    static int count = 0;
    ++count;
    if(timer_.elapsed() > 1000)
    {
        fpsPlane_.setTexts(L"FPS:" + std::to_wstring(count));
        timer_.start();
        count = 0;
    }
    //fpsPlane_.RenderLineBox(0, width_, 0, height_);
    fpsPlane_.render();
}
