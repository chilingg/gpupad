#ifndef TESTCTR_H
#define TESTCTR_H

#include <RController.h>
#include <RWindowCtrl.h>
#include <RPlane.h>
#include <RTextPlane.h>
#include <RTimer.h>
#include <RSprite.h>
#include <RResource/RShaderProgram.h>
#include <RResourceWindow.h>
#include <RResource/RMp3.h>
#include <RAudioStream.h>

class TestCtr : public RController
{
    struct Gamepad
    {
        RInputEvent::JoystickID jid;
        bool connected;
    };

public:
    TestCtr(const std::string &name = "TestCtr", RController *parent = nullptr);
    ~TestCtr() override;

    void control() override;

protected:
    void inputEvent(const RInputEvent *event) override;
    void joystickPresentEvent(RjoystickPresentEvent *event) override;
    void resizeEvent(RResizeEvent *event) override;
    void exitedTreeEvent(RExitedTreeEvent *event) override;
    void enteredTreeEvent(REnteredTreeEvent *event) override;
    void initEvent(RInitEvent *event) override;
    void closeEvent(RCloseEvent *event) override;

    Signal0 closed;

private:
    void FPS();

    Gamepad gamepad_ { RInputEvent::joystick1, false };
    RInputEvent::ButtonAction fullScreenBtn_ = RInputEvent::RELEASE;
    bool fullScreen_ = false;
    RInputEvent::ButtonAction debugWindowBtn_ = RInputEvent::RELEASE;
    RResourceWindow *debugWindow_ = nullptr;
    std::unique_ptr<RPlane> plane_;
    RTextPlane textPlane_;
    int width_;
    int height_;
    RTimer timer_;
    RTextPlane fpsPlane_;
    RSprite sprite_;
    RShaderProgram uiShaders_;
    RPlane bColor_;
    RAudioStream bgm_;
    RAudioStream tick_;
    RInputEvent::ButtonAction bgmBtn_ = RInputEvent::RELEASE;
    RInputEvent::ButtonAction tickBtn_ = RInputEvent::RELEASE;
};

#endif // TESTCTR_H
