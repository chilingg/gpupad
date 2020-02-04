#ifndef RCONTEXT_H
#define RCONTEXT_H

#include "ROpenGL.h"
#include <memory>

namespace Redopera {

// 使用RWindow的场合，初始化由RWindow实例负责

namespace RContext {

struct ContexFormat
{
    bool vSync          = true;     // 垂直同步
    bool forward        = true;     // 前向兼容(清除当前版本不推荐的特性）
    bool debug          = false;    // OpenGL的Debug输出
    bool depth          = false;    // 深度测试
    int versionMajor    = 4;        // OpenGL主版本号
    int versionMinor    = 3;        // OpenGL副版本号
    GLFWwindow *shared  = nullptr;  // 共享上下文
};

thread_local static std::unique_ptr<GLFWwindow, void(*)(GLFWwindow*)> contex(nullptr, glfwDestroyWindow);

bool initialization()
{
    struct glfwHandle
    {
        glfwHandle() { init = glfwInit(); }
        ~glfwHandle() { glfwTerminate(); }
        bool init;
    };
    static glfwHandle glfw;

    return glfw.init;
}

void terminate() { glfwTerminate(); }

GLFWwindow* setContexAsThisThread(const ContexFormat &format = ContexFormat())
{
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, format.versionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, format.versionMinor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, format.forward);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, format.debug);
    glfwWindowHint(GLFW_VISIBLE, false);
    contex.reset(glfwCreateWindow(1, 1, "", nullptr, format.shared));
    if(!contex)
        return nullptr;

    glfwMakeContextCurrent(contex.get());
    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        return nullptr;

    glfwSwapInterval(format.vSync ? 1 : 0);

    return contex.get();
}

void destroyContexAsThisThread() { contex.reset(); }

} // RContext

} // Redopera

#endif // RCONTEXT_H
