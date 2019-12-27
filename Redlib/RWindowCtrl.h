#ifndef RWINDOW_H
#define RWINDOW_H

#include "ROpenGL.h"
#include "RController.h"
#include "RResource/RCursor.h"

#include <vector>
#include <set>

class RWindowCtrl : public RController
{
public:
    static void DefaultWindow();
    static void updateGamepadMappings(std::string path);

    enum ViewportPattern
    {
        FullWindow,
        KeepScale,
        FixedSize
    };

    explicit RWindowCtrl(const std::string &name = "WindowCtrl", RController *parent = nullptr);
    ~RWindowCtrl() override;

    void control() override;
    void translation(const TranslationInfo &info) override;

    int width() const { return width_; }
    int height() const { return height_; }
    //设置函数
    void setWindowSize(int width, int height);
    void setWindowTitle(const std::string &title);
    void setBackgroundColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
    void setBackgroundColor(RColor color);
    void setViewportRatio(double ratio);//视口比例，窗口KeepScale时用
    void setViewportPattern(ViewportPattern pattern);
    void setVSync(bool enable = true);//垂直同步
    void setFullScreenWindow(bool b = true);
    void setWindowMinimumSize(int minW, int minH);
    void setWindowMaximumSize(int maxW, int maxH);
    void setWindowSizeFixed(bool b = true);
    void setWindowDecrate(bool b = true);
    void setWindowFloatOnTop(bool b = true);
    void setWindowIcon(const RImage &img);
    void setCursor(RCursor &cursor);
    void setCursor();
    //查询函数
    double getViewportRatio() const;
    GLFWwindow* getWindowHandle() const;
    bool isFocus() const;
    bool isShouldCloused() const;
    //执行函数
    void closeWindow();
    void trackCursor();
    void untrackCursor();
    void showWindow();
    void hideWindow();
    int exec();

protected:
    //获取与窗口绑定的WindowCtrl
    static RWindowCtrl* getWindowUserCtrl(GLFWwindow *window);
    static GLFWwindow *shareContext;

    std::string getDefaultName() const override;
    void startEvent(RStartEvent *event) override;
    void finishEvent(RFinishEvent *event) override;
    //void closeEvent(RCloseEvent *event) override;

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
    //static void keyboardCollback(GLFWwindow *window, int key, int scancode, int action, int mods);
    //static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void mouseScrollCallback(GLFWwindow *window, double x, double y);
    static void windowFocusCallback(GLFWwindow *window, int focused);
    static void windowCloseCallback(GLFWwindow *window);

    static bool vSync_;//默认锁60FPS

    GLFWwindow *window_;
    void (*eventPool)(); //主线程中为glfwPoolEvent，其余线程中为空

    double viewportRatio_ = 16.0/9.0;
    ViewportPattern viewportPattern = KeepScale;
    int width_ = 960;
    int height_ = 540;
    bool focused_ = true;
};

#endif // RWINDOW_H
