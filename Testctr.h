#ifndef TESTCTR_H
#define TESTCTR_H

#include <RController.h>
#include <RWindowCtrl.h>
#include <RPlane.h>
#include <RTextPlane.h>
#include <RTimer.h>
#include <RSprite.h>
#include <RResource/RShaderProgram.h>

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
    void scrollNotify(double v);

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
    RWindowCtrl *debugWindow_ = nullptr;
    std::unique_ptr<RPlane> plane_;
    RTextPlane textPlane_;
    int width_;
    int height_;
    RTimer timer_;
    RTextPlane fpsPlane_;
    RSprite sprite_;
    RShaderProgram uiShaders_;
    RPlane bColor_;
};

#endif // TESTCTR_H
