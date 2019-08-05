#include "rmaincontrol.h"

RMainControl::RMainControl():
    versionMajor(3),
    versionMinor(3),
    profile(GLFW_OPENGL_CORE_PROFILE),
    width(800),
    height(450),
    title("Redopera"),
    window(nullptr)
{
}

void RMainControl::initialize()
{
    //设置GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, profile);

    //初始化窗口对象
    GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(window == nullptr)
    {
        printErro("Fainled to create GLFW window!");
        glfwTerminate();
        exit(1);
    }
    //GLFW将该context设置为当前线程主context
    glfwMakeContextCurrent(window);
    //对窗口注册一个resize回调函数
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    //注册一个鼠标移动回调函数
    glfwSetCursorPosCallback(window, mouseCallback);

    //初始化GLAD
    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)));
    {
        printErro("Failed to initialize GLAD");
        exit(1);
    }


}

void RMainControl::framebufferSizeCallback(GLFWwindow *, int width, int height)
{
    glViewport(0, 0, width, height);
}

void RMainControl::mouseCallback(GLFWwindow *, double xpos, double ypos)
{
}
