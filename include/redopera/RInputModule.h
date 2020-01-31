#ifndef RINPUTMODULE_H
#define RINPUTMODULE_H

#include "RedoperaConfig.h"
#include "RPoint.h"
#include "RSigslot.h"

#include <map>
#include <vector>

namespace Redopera {

// 按键的注册是在初次Input Event查询时进行的

class RInputEvent;

class RInputModule
{
    friend RInputEvent;

public:
    struct MouseButtonValue
    {
        ButtonAction action = ButtonAction::RELEASE;
        ButtonAction preAction = ButtonAction::RELEASE;
    };

    using KeyValue = MouseButtonValue;

    struct GamepadValue
    {
        GamepadValue(JoystickID jid): jid(jid) { glfwGetGamepadState(static_cast<int>(jid), &status); }

        GLFWgamepadstate status;
        unsigned char preButtons[15];
        JoystickID jid;
    };

    static RInputModule& instance();

    static ButtonAction toButtonAction(unsigned char action);
    static ButtonAction toButtonAction(int action);
    static Keys toKey(int key);
    static MouseButtons toMouseButtons(int button);
    static JoystickID toJoystickID(int jid);

    static const char *gamepadMappingCode0;
    static const char *gamepadMappingCode1;
    static const char *gamepadMappingCode2;

    void updateKeyboardInput(GLFWwindow *window);
    void updateMouseInput(GLFWwindow *window);
    void updateCursorPos(int x, int y);

    void updateGamepad();
    void addGamepad(JoystickID jid);
    bool deleteGamepad(JoystickID jid);

    int gamepadCount();
    bool isValidJid(JoystickID jid);

    RSignal<int> rolled;

private:
    RInputModule() = default;

    std::map<Keys, KeyValue> keyInputs_;
    std::map<MouseButtons, MouseButtonValue> mouseInputs_;
    std::vector<GamepadValue> gamepadInputs_;
    RPoint2 cursorPos_;

    _RSLOT_TAIL_
};

} // Redopera

#endif // RINPUTMODULE_H
