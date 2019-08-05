#ifndef RMAINCONTROL_H
#define RMAINCONTROL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class RMainControl
{
    enum Status{ uninit, normally };
public:
    RMainControl();
    virtual ~RMainControl();
    virtual void initialize();
    int exec();

    void setVersionMajor(int value);
    void setVersionMinor(int value);
    void setProfile(int value);
    void setWindowSize(int width, int height);

private:
    Status status;
    int versionMajor;//主版本号
    int versionMinor;//副版本号
    int profile;//OpenGL模式
    int width;//窗口宽度
    int height;//窗口高度
    const char *title;//窗口标题
    GLFWwindow *window;

    static GLenum _glCheckError_(const char *file, const int line);
#ifndef NO_DEBUGE
#define glCheckError() _glCheckError_(__FILE__, __LINE__)
#endif
    static void printErro(const std::string &error);
    static void errorCallback(int error, const char* description);
    static void framebufferSizeCallback(GLFWwindow *, int width, int height);//窗口resize回调
    static void mouseMoveCallback(GLFWwindow *, double xpos, double ypos);//鼠标回调
    static void keyCallback(GLFWwindow *, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow *, int button, int action, int mods);
    static void mouseScrollCallback(GLFWwindow *, double x, double y);
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

inline void RMainControl::setWindowSize(int width, int height)
{
    if(status == normally)
        glViewport(0, 0, width, height);
    this->width = width;
    this->height = height;
}

inline void RMainControl::printErro(const std::string &error)
{
    std::cerr << error << std::endl;
}

#endif // RMAINCONTROL_H
