#ifndef RKEYEVENT_H
#define RKEYEVENT_H

#include <GLFW/glfw3.h>

class RKeyEvent
{
public:
    static const int KEY_UNKNOWN = GLFW_KEY_UNKNOWN;
    //Printble key
    static const int KEY_SPACE = GLFW_KEY_SPACE;
    static const int KEY_APOSTROPHE = GLFW_KEY_APOSTROPHE;
    static const int KEY_COMMA = GLFW_KEY_COMMA;
    static const int KEY_MINUS = GLFW_KEY_MINUS;
    static const int KEY_PERIOD = GLFW_KEY_PERIOD;
    static const int KEY_SLASH = GLFW_KEY_SLASH;
    static const int KEY_0 = GLFW_KEY_0;
    static const int KEY_1 = GLFW_KEY_1;
    static const int KEY_2 = GLFW_KEY_2;
    static const int KEY_3 = GLFW_KEY_3;
    static const int KEY_4 = GLFW_KEY_4;
    static const int KEY_5 = GLFW_KEY_5;
    static const int KEY_6 = GLFW_KEY_6;
    static const int KEY_7 = GLFW_KEY_7;
    static const int KEY_8 = GLFW_KEY_8;
    static const int KEY_9 = GLFW_KEY_9;
    static const int KEY_SEMICOLON = GLFW_KEY_SEMICOLON;
    static const int KEY_EQUAL = GLFW_KEY_EQUAL;
    static const int KEY_A = GLFW_KEY_A;
    static const int KEY_B = GLFW_KEY_B;
    static const int KEY_C = GLFW_KEY_C;
    static const int KEY_D = GLFW_KEY_D;
    static const int KEY_E = GLFW_KEY_E;
    static const int KEY_F = GLFW_KEY_F;
    static const int KEY_G = GLFW_KEY_G;
    static const int KEY_H = GLFW_KEY_H;
    static const int KEY_I = GLFW_KEY_I;
    static const int KEY_J = GLFW_KEY_J;
    static const int KEY_K = GLFW_KEY_K;
    static const int KEY_L = GLFW_KEY_L;
    static const int KEY_M = GLFW_KEY_M;
    static const int KEY_N = GLFW_KEY_N;
    static const int KEY_O = GLFW_KEY_O;
    static const int KEY_P = GLFW_KEY_P;
    static const int KEY_Q = GLFW_KEY_Q;
    static const int KEY_R = GLFW_KEY_R;
    static const int KEY_S = GLFW_KEY_S;
    static const int KEY_T = GLFW_KEY_T;
    static const int KEY_U = GLFW_KEY_U;
    static const int KEY_V = GLFW_KEY_V;
    static const int KEY_W = GLFW_KEY_W;
    static const int KEY_X = GLFW_KEY_X;
    static const int KEY_Y = GLFW_KEY_Y;
    static const int KEY_Z = GLFW_KEY_Z;
    static const int KEY_LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET;
    static const int KEY_BACKSLASH = GLFW_KEY_BACKSLASH;
    static const int KEY_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET;
    static const int KEY_GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT;
    static const int KEY_WORLD_1 = GLFW_KEY_WORLD_1;
    static const int KEY_WORLD_2 = GLFW_KEY_WORLD_2;
    //Function key
    static const int KEY_ESCAPE = GLFW_KEY_ESCAPE;
    static const int KEY_ENTER = GLFW_KEY_ENTER;
    static const int KEY_TAB = GLFW_KEY_TAB;
    static const int KEY_BACKSPACE = GLFW_KEY_BACKSPACE;
    static const int KEY_INSERT = GLFW_KEY_INSERT;
    static const int KEY_DELETE = GLFW_KEY_DELETE;
    static const int KEY_RIGHT = GLFW_KEY_RIGHT;
    static const int KEY_LEFT = GLFW_KEY_LEFT;
    static const int KEY_DOWN = GLFW_KEY_DOWN;
    static const int KEY_UP = GLFW_KEY_UP;
    static const int KEY_PAGE_UP = GLFW_KEY_PAGE_UP;
    static const int KEY_PAGE_DOWN = GLFW_KEY_PAGE_DOWN;
    static const int KEY_HOME = GLFW_KEY_HOME;
    static const int KEY_END = GLFW_KEY_END;
    static const int KEY_CAPS_LOCK = GLFW_KEY_CAPS_LOCK;
    static const int KEY_SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK;
    static const int KEY_NUM_LOCK = GLFW_KEY_NUM_LOCK;
    static const int KEY_PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN;
    static const int KEY_PAUSE = GLFW_KEY_PAUSE;
    static const int KEY_F1 = GLFW_KEY_F1;
    static const int KEY_F2 = GLFW_KEY_F2;
    static const int KEY_F3 = GLFW_KEY_F3;
    static const int KEY_F4 = GLFW_KEY_F4;
    static const int KEY_F5 = GLFW_KEY_F5;
    static const int KEY_F6 = GLFW_KEY_F6;
    static const int KEY_F7 = GLFW_KEY_F7;
    static const int KEY_F8 = GLFW_KEY_F8;
    static const int KEY_F9 = GLFW_KEY_F9;
    static const int KEY_F10 = GLFW_KEY_F10;
    static const int KEY_F11 = GLFW_KEY_F11;
    static const int KEY_F12 = GLFW_KEY_F12;
    static const int KEY_F13 = GLFW_KEY_F13;
    static const int KEY_F14 = GLFW_KEY_F14;
    static const int KEY_F15 = GLFW_KEY_F15;
    static const int KEY_F16 = GLFW_KEY_F16;
    static const int KEY_F17 = GLFW_KEY_F17;
    static const int KEY_F18 = GLFW_KEY_F18;
    static const int KEY_F19 = GLFW_KEY_F19;
    static const int KEY_F20 = GLFW_KEY_F20;
    static const int KEY_F21 = GLFW_KEY_F21;
    static const int KEY_F22 = GLFW_KEY_F22;
    static const int KEY_F23 = GLFW_KEY_F23;
    static const int KEY_F24 = GLFW_KEY_F24;
    static const int KEY_F25 = GLFW_KEY_F25;
    static const int KEY_KP_0 = GLFW_KEY_KP_0;
    static const int KEY_KP_1 = GLFW_KEY_KP_1;
    static const int KEY_KP_2 = GLFW_KEY_KP_2;
    static const int KEY_KP_3 = GLFW_KEY_KP_3;
    static const int KEY_KP_4 = GLFW_KEY_KP_4;
    static const int KEY_KP_5 = GLFW_KEY_KP_5;
    static const int KEY_KP_6 = GLFW_KEY_KP_6;
    static const int KEY_KP_7 = GLFW_KEY_KP_7;
    static const int KEY_KP_8 = GLFW_KEY_KP_8;
    static const int KEY_KP_9 = GLFW_KEY_KP_9;
    static const int KEY_KP_DECIMAL = GLFW_KEY_KP_DECIMAL;
    static const int KEY_KP_DIVIDE = GLFW_KEY_KP_DIVIDE;
    static const int KEY_KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY;
    static const int KEY_KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT;
    static const int KEY_KP_ADD = GLFW_KEY_KP_ADD;
    static const int KEY_KP_ENTER = GLFW_KEY_KP_ENTER;
    static const int KEY_KP_EQUAL = GLFW_KEY_KP_EQUAL;
    static const int KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
    static const int KEY_LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL;
    static const int KEY_LEFT_ALT = GLFW_KEY_LEFT_ALT;
    static const int KEY_LEFT_SUPER = GLFW_KEY_LEFT_SUPER;
    static const int KEY_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT;
    static const int KEY_RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL;
    static const int KEY_RIGHT_ALT = GLFW_KEY_RIGHT_ALT;
    static const int KEY_RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER;
    static const int KEY_MENU = GLFW_KEY_MENU;

    //Modifier key flags
    static const int No_Modifier = 0;
    static const int Shift_Modifier = GLFW_MOD_SHIFT;
    static const int Control_Modifier = GLFW_MOD_CONTROL;
    static const int Alt_Modifier = GLFW_MOD_ALT;
    static const int Super_Modifier = GLFW_MOD_SUPER;

    RKeyEvent(int key, int modifiers);
    int key() const;
    int modifiers() const;

private:
    const int KEY;
    const int MODIFIERS;
};

inline RKeyEvent::RKeyEvent(int key, int modifiers):
    KEY(key),
    MODIFIERS(modifiers)
{

}

inline int RKeyEvent::key() const
{
    return KEY;
}

inline int RKeyEvent::modifiers() const
{
    return MODIFIERS;
}

#endif // RKEYEVENT_H
