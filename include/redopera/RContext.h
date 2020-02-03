#ifndef RCONTEXT_H
#define RCONTEXT_H

#include "ROpenGL.h"
#include  <memory>

namespace Redopera {

// 使用RWindow的场合，初始化与结束由RWindow实例负责

namespace RContext {

struct ContexFormat
{
    bool vSync          = true;     // 垂直同步
    bool forward        = true;     // 前向兼容(清除当前版本不推荐的特性）
    bool debug          = false;    // OpenGL的Debug输出
    bool depth          = true;     // 深度测试
    int versionMajor    = 4;        // OpenGL主版本号
    int versionMinor    = 3;        // OpenGL副版本号
};

GLFWwindow* getContexAsThisThread(GLFWwindow *shared)
{
    thread_local static std::unique_ptr<GLFWwindow, void(*)(GLFWwindow*)> contex(nullptr, glfwDestroyWindow);

    if(contex == nullptr)
    {
        contex.reset(glfwCreateWindow(1, 1, "", nullptr, shared));

        if(!contex) return nullptr;

        glfwMakeContextCurrent(contex.get());
        if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
            return nullptr;
    }

    return contex.get();
}

GLFWwindow* getContexAsThisThread() { return getContexAsThisThread(nullptr); }

static bool initialization(const ContexFormat &format = ContexFormat())
{
    // 初始化GLFW
    if(!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, format.versionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, format.versionMinor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, format.forward);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, format.debug);

    return getContexAsThisThread() != nullptr;
}

static void terminate() { glfwTerminate(); }

} // RContext

} // Redopera

#endif // RCONTEXT_H
