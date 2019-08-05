#include "rmaincontrol.h"
#include "rdebug.h"

RMainControl::RMainControl():
    status(uninit),
    versionMajor(3),
    versionMinor(3),
    profile(GLFW_OPENGL_CORE_PROFILE),
    width(800),
    height(450),
    title("Redopera")
{
}

RMainControl::~RMainControl()
{

}

void RMainControl::initialize()
{
    if(status == uninit)
    {
        //设置GLFW
        if(!glfwInit())
            exit(EXIT_FAILURE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, profile);

        //初始化窗口对象
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if(window == nullptr)
        {
            printErro("Fainled to create GLFW window!");
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        //GLFW将该context设置为当前线程主context
        glfwMakeContextCurrent(window);

        glfwSetErrorCallback(errorCallback);
        //对窗口注册一个resize回调函数
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        //注册一个鼠标移动回调函数
        glfwSetCursorPosCallback(window, mouseMoveCallback);
        //键盘回调
        glfwSetKeyCallback(window, keyCallback);
        //鼠标点击
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
        //鼠标滚轮
        glfwSetScrollCallback(window, mouseScrollCallback);

        //初始化GLAD
        if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            printErro("Failed to initialize GLAD");
            exit(EXIT_FAILURE);
        }

        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glEnable(GL_DEPTH_TEST);//开启深度测试
        glEnable(GL_BLEND);//启用半透明
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//设置源与目标的混个因子

        glCheckError();
        status = normally;
    }
}

int RMainControl::exec()
{
    int flag = 0;

    if(status == uninit)
        return -1;

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //交换颜色缓冲
        glfwSwapBuffers(window);
        //检查GLFW事件触发
        glfwPollEvents();

        glCheckError();
    }

    glfwTerminate();
    return flag;
}

GLenum RMainControl::_glCheckError_(const char *file, const int line)
{
    GLenum errorCode;
    while((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch(errorCode)
        {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW:
            error = "STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            error = "STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        }
        error = error + ": In "  + file + '(' + std::to_string(line) + ')';
        printErro(error);
    }

    return errorCode;
}

void RMainControl::setWindowSize(int width, int height)
{
    if(status == normally)
        glViewport(0, 0, width, height);
    this->width = width;
    this->height = height;
}

void RMainControl::errorCallback(int error, const char *description)
{
    printErro("In errorCallback!");
    printErro(description);
}

void RMainControl::framebufferSizeCallback(GLFWwindow *, int width, int height)
{
    glViewport(0, 0, width, height);
}

void RMainControl::mouseMoveCallback(GLFWwindow *, double xpos, double ypos)
{
    RDebug() << 'x';
}

void RMainControl::keyCallback(GLFWwindow *, int key, int scancode, int action, int mods)
{

}

void RMainControl::mouseButtonCallback(GLFWwindow *, int button, int action, int mods)
{

}

void RMainControl::mouseScrollCallback(GLFWwindow *, double x, double y)
{

}
