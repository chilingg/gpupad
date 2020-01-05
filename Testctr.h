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
#include <RThread.h>

class TestCtr : public RController
{
    struct Gamepad
    {
        RInputModule::JoystickID jid;
        bool connected;
    };

public:
    TestCtr(const std::string &name = "TestCtr", RController *parent = nullptr);
    ~TestCtr() override;

    void control() override;
    void translation(const TranslationInfo &info) override;

protected:
    void inputEvent(RInputEvent *event) override;
    void exitedTreeEvent(RExitedTreeEvent *event) override;
    void enteredTreeEvent(REnteredTreeEvent *event) override;
    void startEvent(RStartEvent *event) override;
    void finishEvent(RFinishEvent *event) override;

    RSignal<> closed;

private:
    void FPS();

    Gamepad gamepad_ { RInputModule::JOYSTICK_1, false };
    RInputModule::ButtonAction fullScreenBtn_ = RInputModule::RELEASE;
    bool fullScreen_ = false;
    RInputModule::ButtonAction debugWindowBtn_ = RInputModule::RELEASE;
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
    RTimer bgmSetTimer_;
    RAudioStream tick_;

    RThread reWindowThread_;

    _RSLOT_TAIL_
};

#endif // TESTCTR_H
