#include "rmaincontrol.h"
#include "rdebug.h"
#include <fstream>
#include <sstream>

RMainControl::Joysticks RMainControl::joysticks{};

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
        //初始化GLFW
        if(!glfwInit())
            exit(EXIT_FAILURE);
        //设置上下文
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, profile);

        //初始化窗口对象并创建上下文
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
        //joystick连接回调
        glfwSetJoystickCallback(joystickPresentCallback);

        //加载手柄映射
        std::ifstream file;
        // 保证ifstream对象可以抛出异常：
        file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        file.open("../redopera/data/gamecontrollerdb.txt");
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();
        const char* mapping = ss.str().c_str();
        glfwUpdateGamepadMappings(mapping);

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

        //手柄连接检测
        checkJoysticksPresent();

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
        //检查手柄输入
        joystickCheckInput();

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

void RMainControl::checkJoysticksPresent()
{
    for(int i = GLFW_JOYSTICK_1; i <= GLFW_MOUSE_BUTTON_LAST; ++i)
    {
        if(glfwJoystickIsGamepad(i))
        {
            joysticks[i] = true;
        }
    }
    RDebug() << "In " << __LINE__ << joysticks.size() << "number";
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
    RDebug() << 'f';
    glViewport(0, 0, width, height);
}

void RMainControl::mouseMoveCallback(GLFWwindow *, double xpos, double ypos)
{
    //RDebug() << xpos << ypos;
}

void RMainControl::keyCallback(GLFWwindow *, int key, int scancode, int action, int mods)
{
    //const char *str_ch = glfwGetKeyName(GLFW_KEY_UNKNOWN, scancode);
    //printf("glfwGetKeyName:%s\n", str_ch);
    //RDebug() << key;
}

void RMainControl::mouseButtonCallback(GLFWwindow *, int button, int action, int mods)
{
    RDebug() << 'b';
}

void RMainControl::mouseScrollCallback(GLFWwindow *, double x, double y)
{
    RDebug() << x << y;
}

void RMainControl::joystickPresentCallback(int jid, int event)
{
    if (event == GLFW_CONNECTED)
    {
        joysticks[jid] = true;
    }
    else if (event == GLFW_DISCONNECTED)
    {
        joysticks.erase(jid);
    }
}

void RMainControl::joystickCheckInput()
{
    GLFWgamepadstate status;
    for(auto jid : joysticks)
    {
        if(glfwGetGamepadState(jid.first, &status))
        {
            //GLFW_GAMEPAD_BUTTON_A
            unsigned size = sizeof(status.buttons)/sizeof(status.buttons[0]);
            for(unsigned i = 0; i < size; ++i)
            {
                if(status.buttons[i])
                    RDebug() << i;
            }
        }
    }
}
