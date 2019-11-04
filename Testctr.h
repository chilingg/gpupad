#ifndef TESTCTR_H
#define TESTCTR_H

#include <RController.h>

class TestCtr : public RController
{
    struct Gamepad
    {
        RInputEvent::JoystickID jid;
        bool connected;
    };

public:
    TestCtr(const std::string &name = "TestCtr", RController *parent = nullptr);

    void control() override;

protected:
    void inputEvent(const RInputEvent *event) override;
    void joystickPresentEvent(RjoystickPresentEvent *event) override;
    void resizeEvent(RResizeEvent *event) override;
    void exitedTreeEvent(RExitedTreeEvent *event) override;
    void enteredTreeEvent(REnteredTreeEvent *event) override;
    void initEvent(RInitEvent *event) override;
    void closeEvent(RCloseEvent *event) override;

    sigslot::signal0<> closed;

private:
    Gamepad gamepad_ { RInputEvent::joystick1, false };
};

#endif // TESTCTR_H
