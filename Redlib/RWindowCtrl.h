#ifndef RWINDOW_H
#define RWINDOW_H

#include "ROpenGL.h"
#include "RController.h"

#include <vector>
#include <set>

class RWindowCtrl : public RController
{
public:
    enum ViewportPattern
    {
        FullWindow,
        KeepScale
    };

    static void setVSync(bool enable);

    RWindowCtrl(const std::string &name = "WindowCtrl", RController *parent = nullptr);
    ~RWindowCtrl() override;

    void control() override;

    //设置函数
    //GLFWwindow *setWindowAsCurrent();
    void setWindowSize(int width, int height);
    void setBackground(int r, int g, int b);
    void setViewportRatio(double ratio);
    void setViewportPattern(ViewportPattern pattern);
    //查询函数
    //执行函数
    void closeWindow();
    void enableGamepad();
    void disableGamepad();
    bool updataGamepadMappings(std::string path);
    void trackCursor();
    void untrackCursor();
    void showWindow();
    void hideWindow();

protected:
    std::string getDefaultName() const override;
    void initEvent(RInitEvent *event) override;
    void closeEvent(RCloseEvent *event) override;
    
private:
    //与窗口绑定的回调
    static void errorCallback(int error, const char* description);
    static void resizeCallback(GLFWwindow *window, int width, int height);
    static void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos);
    //键盘回调参数 = key：激发的键值，scancode：键值的系统扫描码，
    //action：GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT，modes：ALT，CTRL，SHIFT，META等
    static void keyboardCollback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void mouseWheelCallback(GLFWwindow *window, double x, double y);
    static void joystickPresentCallback(int jid, int event);

    static RWindowCtrl* getWindowCtrl(const GLFWwindow *window);

    static std::vector<RWindowCtrl*> windowCtrls_;
    static int count;
    static bool gamepadModule_;
    static bool vSync_;//默认锁60FPS
    static std::set<RInputEvent::JoystickID> gamepads;

    GLFWwindow *window_;
    RInputEvent inputEvent;

    float backgroundColor[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    int width_ = 960;
    int height_ = 540;
    double viewportRatio_ = 16.0/9.0;
    ViewportPattern viewportPattern = KeepScale;
};

#endif // RWINDOW_H
