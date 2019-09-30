#include <RWindow.h>
#include <RDebug.h>
#include <RResource.h>

#include <RKeyEvent.h>
#include <RMouseEvent.h>
#include <RWheelEvent.h>
#include <RJoystickEvent.h>

#include <fstream>
#include <sstream>

RController *RWindow::root(nullptr);
RWindow::Joysticks RWindow::joysticks;
RResizeEvent::Pattern RWindow::windowPattern = RResizeEvent::Keep;

RWindow::RWindow(int vMajor, int vMinor, int profile):
    width(800),
    height(450),
    title("Redopera"),
    window(nullptr),
    cursorTrack(false),
    vSync(1)
{
    //初始化GLFW
    if(!glfwInit())
    {
        printErro("Failed to initialize GLFW");
        exit(EXIT_FAILURE);
    }
    //设置上下文
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, vMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, vMinor);
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

    //初始化GLAD
    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        printErro("Failed to initialize GLAD");
        exit(EXIT_FAILURE);
    }

}

RWindow::~RWindow()
{
    glfwTerminate();
}

bool RWindow::initialize()
{
    root->initialization();

    glfwSetWindowSize(window, width, height);
    RResizeEvent e(width, height);
    root->dispatcherResizeEvent(&e);

    //垂直同步
    glfwSwapInterval(vSync);

    glfwSetErrorCallback(errorCallback);
    //对窗口注册一个resize回调函数
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    //注册一个鼠标移动回调函数
    if(cursorTrack)
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
    std::string mapping = RResource::openTextFile("../Redopera/data/gamecontrollerdb.txt");
    glfwUpdateGamepadMappings(mapping.c_str());

    //需手动检测一次手柄连接，避免开始之前的连接无法检测（需要root控制已设置）
    for(int i = GLFW_JOYSTICK_1; i <= GLFW_MOUSE_BUTTON_LAST; ++i)
    {
        if(glfwJoystickIsGamepad(i))
        {
            RDebug() << 1;
            joystickPresentCallback(i, GLFW_CONNECTED);
        }
    }

    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    //默认开启面剔除
    //glEnable(GL_CULL_FACE);
    //默认开启混合（半透明）
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    RDebug() << glGetString(GL_VERSION);

    glCheckError();

    return true;
}

int RWindow::exec(RController *root)
{
    if(!root)
    {
        printErro("Lack of controller!");
        return -1;
    }
    this->root = root;

    if(!initialize())
    {
        printErro("Initialization filed!");
        exit(EXIT_FAILURE);
    }

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //检查GLFW事件触发
        glfwPollEvents();
        //手柄输入检测
        if(!joysticks.empty())
        {
            for(auto &jid : joysticks)
                joystickInputCheck(jid);
        }
        //调用控制
        root->control();

        glCheckError();

        //交换颜色缓冲
        glfwSwapBuffers(window);
    }

    root->close();
    glfwDestroyWindow(window);
    window = nullptr;

    return 0;
}

GLenum RWindow::_glCheckError_(const char *file, const int line)
{
    GLenum errorCode = 0;

#ifndef RO_NO_DEBUGE
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

#endif
    return errorCode;
}

void RWindow::setWindowSize(int width, int height)
{
    this->width = width;
    this->height = height;
}

void RWindow::errorCallback(int error, const char *description)
{
    printErro("In errorCallback!");
    printErro(description);
}

void RWindow::framebufferSizeCallback(GLFWwindow *, int width, int height)
{
    RResizeEvent e(width, height);
    root->dispatcherResizeEvent(&e);

    if(windowPattern == RResizeEvent::Keep)
    {
        bool wh = width/16.0 < height/9.0;
        double base = wh ? width/16.0 : height/9.0;
        int newW = static_cast<int>(base*16);
        int newH = static_cast<int>(base*9);

        if(wh)
            glViewport(0, (height-newH)/2.0, newW, newH);
        else
            glViewport((width-newW)/2.0, 0, newW, newH);

        RResizeEvent e(newW, newH);
        root->dispatcherResizeEvent(&e);
    }
    else
    {
        glViewport(0, 0, width, height);
        RResizeEvent e(width, height, windowPattern);
        root->dispatcherResizeEvent(&e);
    }
}

void RWindow::mouseMoveCallback(GLFWwindow *, double xpos, double ypos)
{
    int x = static_cast<int>(xpos);
    int y = static_cast<int>(ypos);

    RMouseEvent event(x, y);
    root->dispatcherInputEvent(&event, RController::MouseMoveEvent);
}

void RWindow::keyCallback(GLFWwindow *window, int key, int, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    RKeyEvent event(key, mods);
    if(action == GLFW_PRESS)
        root->dispatcherInputEvent(&event, RController::KeyPressEvent);
    else if(action == GLFW_RELEASE)
        root->dispatcherInputEvent(&event, RController::KeyReleaseEvent);
}

void RWindow::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int x = static_cast<int>(xpos);
    int y = static_cast<int>(ypos);

    RMouseEvent event(x, y, button, mods);
    if(action == GLFW_PRESS)
        root->dispatcherInputEvent(&event, RController::MousePressEvent);
    else if(action == GLFW_RELEASE)
        root->dispatcherInputEvent(&event, RController::MouseReleaseEvent);
}

void RWindow::mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int x = static_cast<int>(xpos);
    int y = static_cast<int>(ypos);
    int offset = yOffset > 0 ? 1 : -1;

    RWheelEvent event(x, y, offset);
    root->dispatcherInputEvent(&event, RController::WheelEvent);
}

void RWindow::joystickPresentCallback(int jid, int event)
{
    RJoystickEvent e(jid);
    root->dispatcherjoystickEvent(&e, RController::JoystickPresentEvent);

    if(event == GLFW_CONNECTED)
    {
        joysticks.emplace_back(jid);
        RDebug() << __LINE__ << "Line: Connected" << "joystick jid=" << jid;
    }
    else if(event == GLFW_DISCONNECTED)
    {
        joysticks.emplace_back(jid);
        RDebug() << __LINE__ << "Line: Disconnected" << "joystick jid=" << jid;
    }
}

void RWindow::joystickInputCheck(RJoystick &joy)
{
    GLFWgamepadstate status;
    if(glfwGetGamepadState(joy.jid(), &status))
    {
        //GLFW_GAMEPAD_BUTTON_A
        for(unsigned i = 0; i <= RJoystick::GAMEPAD_BUTTON_LAST; ++i)
        {
            if(status.buttons[i] != joy.button(i))
            {
                //RDebug() << i;
                RJoystickEvent event(joy.jid());
                status.buttons[i] == GLFW_PRESS ? event.setButton(i, true) : event.setButton(i, false);
                root->dispatcherjoystickEvent(&event, RController::JoystickInput);
            }
        }

        static const float inaccaracy = 0.1f;
        for(unsigned i = 0; i <= RJoystick::GAMEPAD_AXIS_RIGHT_Y; ++i)
        {
            if(status.axes[i] > inaccaracy || status.axes[i] < -inaccaracy)
            {
                RJoystickEvent event(joy.jid());
                event.setAxis(i, status.axes[i]);
                root->dispatcherjoystickEvent(&event, RController::JoystickInput);
            }
        }

        static const float lStart = -1;
        static const float rStart = -1;
        if(status.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] > lStart)
        {
            RJoystickEvent event(joy.jid());
            event.setAxis(RJoystick::GAMEPAD_AXIS_LEFT_TRIGGER, status.axes[RJoystick::GAMEPAD_AXIS_LEFT_TRIGGER]);
            root->dispatcherjoystickEvent(&event, RController::JoystickInput);
            //RDebug() << status.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] << "L";
        }
        if(status.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] > rStart)
        {
            RJoystickEvent event(joy.jid());
            event.setAxis(RJoystick::GAMEPAD_AXIS_RIGHT_TRIGGER, status.axes[RJoystick::GAMEPAD_AXIS_RIGHT_TRIGGER]);
            root->dispatcherjoystickEvent(&event, RController::JoystickInput);
            //RDebug() << status.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] << "R";
        }

        joy.update();
    }
}
