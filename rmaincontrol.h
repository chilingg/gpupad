#ifndef RMAINCONTROL_H
#define RMAINCONTROL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class RMainControl
{
public:
    RMainControl();
    virtual ~RMainControl();
    virtual void initialize();

    void setVersionMajor(int value);
    void setVersionMinor(int value);
    void setProfile(int value);
    void setWindowSize(int width, int height);

private:
    int versionMajor;//主版本号
    int versionMinor;//副版本号
    int profile;//OpenGL模式
    int width;//窗口宽度
    int height;//窗口高度
    const char *title;//窗口标题
    GLFWwindow *window;
    
    void printErro(const char *erro);
    static void framebufferSizeCallback(GLFWwindow *, int width, int height);
    static void mouseCallback(GLFWwindow *, double xpos, double ypos);
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
    this->width = width;
    this->height = height;
}

inline void RMainControl::printErro(const char *erro)
{
    std::cerr << erro << std::endl;
}

#endif // RMAINCONTROL_H
