#ifndef TESTCTR_H
#define TESTCTR_H

#include <RController.h>
#include <RWindowCtrl.h>

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
    Gamepad gamepad_ { RInputEvent::joystick1, false };
    RInputEvent::ButtonAction fullScreenBtn_ = RInputEvent::RELEASE;
    bool fullScreen_ = false;
    RInputEvent::ButtonAction debugWindowBtn_ = RInputEvent::RELEASE;
    RWindowCtrl *debugWindow_ = nullptr;
};

#endif // TESTCTR_H
