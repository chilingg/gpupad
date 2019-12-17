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
        RInputRegistry::JoystickID jid;
        bool connected;
    };

public:
    TestCtr(const std::string &name = "TestCtr", RController *parent = nullptr);
    ~TestCtr() override;

    void control() override;

protected:
    void inputEvent(const RInputRegistry *event) override;
    void joystickPresentEvent(RjoystickPresentEvent *event) override;
    void resizeEvent(RResizeEvent *event) override;
    void exitedTreeEvent(RExitedTreeEvent *event) override;
    void enteredTreeEvent(REnteredTreeEvent *event) override;
    void initEvent(RInitEvent *event) override;
    void closeEvent(RCloseEvent *event) override;

    Signal0 closed;

private:
    void FPS();

    Gamepad gamepad_ { RInputRegistry::joystick1, false };
    RInputRegistry::ButtonAction fullScreenBtn_ = RInputRegistry::RELEASE;
    bool fullScreen_ = false;
    RInputRegistry::ButtonAction debugWindowBtn_ = RInputRegistry::RELEASE;
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
    RInputRegistry::ButtonAction bgmBtn_ = RInputRegistry::RELEASE;
    RInputRegistry::ButtonAction tickBtn_ = RInputRegistry::RELEASE;
};

#endif // TESTCTR_H
