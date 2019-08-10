#ifndef RMAINCONTROL_H
#define RMAINCONTROL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>

class RMainControl
{
    using Joysticks = std::map<int, bool>;
    enum Status{ uninit, normally };
public:
    RMainControl();
    ~RMainControl();
    void initialize();
    int exec();

    void setVersionMajor(int value);
    void setVersionMinor(int value);
    void setProfile(int value);
    void setWindowSize(int width, int height);
    void enableGLCapabilities(GLenum cap);
    void disableGLCapabilities(GLenum cap);

private:
    static Joysticks joysticks;

    Status status;
    int versionMajor;//主版本号
    int versionMinor;//副版本号
    int profile;//OpenGL模式
    int width;//窗口宽度
    int height;//窗口高度
    const char *title;//窗口标题
    GLFWwindow *window;

    void checkJoysticksPresent();

    static GLenum _glCheckError_(const char *file, const int line);
    #define glCheckError() _glCheckError_(__FILE__, __LINE__)

    //回调与事件相关函数
    static void errorCallback(int error, const char* description);
    static void framebufferSizeCallback(GLFWwindow *, int width, int height);//窗口resize回调
    static void mouseMoveCallback(GLFWwindow *, double xpos, double ypos);//鼠标回调
    //key：激发的键值，scancode：键值的系统扫描码，action：GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT，modes：ALT，CTRL，SHIFT，META等
    static void keyCallback(GLFWwindow *, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow *, int button, int action, int mods);
    static void mouseScrollCallback(GLFWwindow *, double x, double y);
    static void joystickPresentCallback(int jid, int event);
    static void joystickCheckInput();
};

inline void RMainControl::setVersionMajor(int value)
{
    versionMajor = value;
}

inline void RMainControl::setVersionMinor(int value)
{
    versionMinor = value;
}

inline void RMainControl::setProfile(int value)
{
    profile = value;
}

inline void RMainControl::enableGLCapabilities(GLenum cap)
{
    /*
    glEnable(GL_DEPTH_TEST);//开启深度测试
    glEnable(GL_BLEND);//启用半透明
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//设置源与目标的混个因子
    */
    glEnable(cap);
}

inline void RMainControl::disableGLCapabilities(GLenum cap)
{
    glDisable(cap);
}

#endif // RMAINCONTROL_H
