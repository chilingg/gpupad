#ifndef RWINDOW_H
#define RWINDOW_H

#include "ROpenGL.h"
#include "RController.h"

#include <vector>
#include <set>

class RWindowCtrl : public RController
{
public:
    static void DefaultWindow();
    static void updataGamepadMappings(std::string path);

    enum ViewportPattern
    {
        FullWindow,
        KeepScale
    };

    explicit RWindowCtrl(const std::string &name = "WindowCtrl", RController *parent = nullptr);
    ~RWindowCtrl() override;

    void control() override;

    //设置函数
    void setWindowSize(int width, int height);
    void setWindowTitle(const std::string &title);
    void setBackground(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
    void setViewportRatio(double ratio);//视口比例，窗口KeepScale时用
    void setViewportPattern(ViewportPattern pattern);
    void setVSync(bool enable);//垂直同步
    void setFullScreenWindow(bool b);
    void setWindowSizeLimits(int minW, int minH, int maxW, int maxH);
    void setWindowSizeFixed(bool b);
    void setWindowDecrate(bool b);
    void setWindowFloatOnTop(bool b);
    //查询函数
    double getViewportRatio() const;
    bool isShouldCloused() const;
    //执行函数
    void closeWindow();
    void trackCursor();
    void untrackCursor();
    void showWindow();
    void hideWindow();

    Signal1<double> scrolled;

protected:
    std::string getDefaultName() const override;
    void initEvent(RInitEvent *event) override;
    void closeEvent(RCloseEvent *event) override;
    //获取与窗口绑定的WindowCtrl
    static RWindowCtrl* getWindowUserCtrl(GLFWwindow *window);

private:
    //与窗口绑定的回调
    static void glfwErrorCallback(int error, const char* description);
    static void openglDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                           GLsizei length, const GLchar *message, const void *userParam);
    static void joystickPresentCallback(int jid, int event);
    static void resizeCallback(GLFWwindow *window, int width, int height);
    static void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos);
    //键盘回调参数 = key：激发的键值，scancode：键值的系统扫描码，
    //action：GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT，modes：ALT，CTRL，SHIFT，META等
    static void keyboardCollback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void mouseScrollCallback(GLFWwindow *window, double x, double y);
    static void windowFocusCallback(GLFWwindow *window, int focused);

    static bool vSync_;//默认锁60FPS
    static int count;

    GLFWwindow *window_;
    //RInputEvent inputEvent;

    double viewportRatio_ = 16.0/9.0;
    ViewportPattern viewportPattern = KeepScale;
    int width_ = 960;
    int height_ = 540;
    bool focused_ = true;
};

#endif // RWINDOW_H
