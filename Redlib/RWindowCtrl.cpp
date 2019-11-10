#include "RWindowCtrl.h"
#include "RResource/RResource.h"

#include "RDebug.h"

int RWindowCtrl::count = 0;
bool RWindowCtrl::vSync_ = true;

RWindowCtrl::RWindowCtrl(const std::string &name, RController *parent):
    RController(name, parent),
    window_(nullptr)
{
    if(++count == 1)
    {
        //glfw错误回调
        glfwSetErrorCallback(errorCallback);
        //初始化GLFW
        if(!glfwInit())
        {
            printError("Failed to initialize GLFW");
            parentToNull();
            exit(EXIT_FAILURE);
        }
        //加载手柄映射
        updataGamepadMappings(":/data/gamecontrollerdb.txt");
        //手柄连接回调
        glfwSetJoystickCallback(joystickPresentCallback);

        //需手动检测一次手柄连接，检测之前已连接的手柄
        for(int i = RInputEvent::joystick1; i <= RInputEvent::joystickMaxNum; ++i)
        {
            if(glfwJoystickIsGamepad(i))
                gamepads.insert(RInputEvent::toJoystickID(i));
        }
    }

    //同线程窗口统一共享上下文
    GLFWwindow *share = glfwGetCurrentContext();
    window_ = glfwCreateWindow(width_, height_, "Redopera", nullptr, share);
    if(!window_)
    {
        printError("Fainled to create GLFW window!");
        parentToNull();
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    //绑定上下文与this指针
    glfwSetWindowUserPointer(window_, this);

    //如果当前线程之前没有窗口创建，则将该context设置为当前线程主context
    if(!share)
    {
        glfwMakeContextCurrent(window_);
        if(count == 1)
        {
            //初始化glad
            if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
            {
                printError("Failed to initialize GLAD");
                parentToNull();
                glfwTerminate();
                exit(EXIT_FAILURE);
            }
#ifndef R_NO_DEBUG
            RDebug() << glGetString(GL_VERSION);
#endif
        }
        glViewport(0, 0, width_, height_);
        //设置混合函数 Ps:混合需另外开启
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //默认开启垂直同步
        glfwSwapInterval(true);
    }

    glfwSetFramebufferSizeCallback(window_, resizeCallback);
    glfwSetKeyCallback(window_, keyboardCollback);
    glfwSetMouseButtonCallback(window_, mouseButtonCallback);
    glfwSetScrollCallback(window_, mouseScrollCallback);
    glfwSetCursorPosCallback(window_, mouseMoveCallback);

    //GLFW事件触发
    poolEvent = &glfwPollEvents;
}

RWindowCtrl::~RWindowCtrl()
{
    assert(window_);
    glfwDestroyWindow(window_);
}

void RWindowCtrl::control()
{
    GLFWwindow *before = glfwGetCurrentContext();
    glfwMakeContextCurrent(window_);

    //清除颜色缓存
    glClearBufferfv(GL_COLOR, 0, backgroundColor);
    //调动子结点控制
    allChildrenActive();
    glfwSwapBuffers(window_);

    //切换为之前的上下文
    glfwMakeContextCurrent(before);

    if(glfwWindowShouldClose(window_))
        inactive();
}

void RWindowCtrl::setWindowSize(int width, int height)
{
    glfwSetWindowSize(window_, width, height);
}

void RWindowCtrl::setWindowTitle(const std::string &title)
{
    glfwSetWindowTitle(window_, title.c_str());
}

void RWindowCtrl::setBackground(int r, int g, int b)
{
    backgroundColor[0] = r / 255.0f;
    backgroundColor[1] = g / 255.0f;
    backgroundColor[2] = b / 255.0f;
}

void RWindowCtrl::setViewportRatio(double ratio)
{
    viewportRatio_ = ratio;
}

void RWindowCtrl::setViewportPattern(RWindowCtrl::ViewportPattern pattern)
{
    viewportPattern = pattern;
}

void RWindowCtrl::setVSync(bool enable)
{
    vSync_ = enable ? 1 : 0;//参数为1锁60帧
    glfwSwapInterval(vSync_);
}

double RWindowCtrl::getViewportRatio() const
{
    return viewportRatio_;
}

void RWindowCtrl::DefaultWindow()
{
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//set主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//set副版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//use核心模式
}

void RWindowCtrl::WindowDecorate(bool enable)
{
    glfwWindowHint(GLFW_DECORATED, enable ? GLFW_TRUE : GLFW_FALSE);
}

void RWindowCtrl::updataGamepadMappings(std::string path)
{
    try{
        std::string mappingCode = RResource::getTextFileContent(path);
        glfwUpdateGamepadMappings(mappingCode.c_str());
    }
    catch(...)
    {
        printError("Failed to updata gamepad mapping! In path: " + path + '\n' +
                   "To https://github.com/gabomdq/SDL_GameControllerDB download gamecontrollerdb.txt file.");
        //加载内置的手柄映射
        std::string mappingCode = std::string() + RInputEvent::gamepadMappingCode0
                + RInputEvent::gamepadMappingCode1 + RInputEvent::gamepadMappingCode2;
        glfwUpdateGamepadMappings(mappingCode.c_str());
    }
}

void RWindowCtrl::closeWindow()
{
    glfwSetWindowShouldClose(window_, GLFW_TRUE);
}

void RWindowCtrl::trackCursor()
{
    glfwSetCursorPosCallback(window_, mouseMoveCallback);
}

void RWindowCtrl::untrackCursor()
{
    glfwSetCursorPosCallback(window_, [](GLFWwindow *, double , double) {});
}

void RWindowCtrl::showWindow()
{
    glfwShowWindow(window_);
}

void RWindowCtrl::hideWindow()
{
    glfwHideWindow(window_);
}

std::string RWindowCtrl::getDefaultName() const
{
    return "WindowCtrl";
}

void RWindowCtrl::initEvent(RInitEvent *)
{
}

void RWindowCtrl::closeEvent(RCloseEvent *)
{
}

RWindowCtrl *RWindowCtrl::getWindowUserCtrl(GLFWwindow *window)
{
    return static_cast<RWindowCtrl*>(glfwGetWindowUserPointer(window));
}

void RWindowCtrl::errorCallback(int error, const char *description)
{
    printError("Error " + std::to_string(error) + ": " + description);
}

void RWindowCtrl::joystickPresentCallback(int jid, int event)
{
    RInputEvent::JoystickID J = RInputEvent::toJoystickID(jid);
    bool isConnected = event == RInputEvent::joystickConnected ? true : false;
    RWindowCtrl *wctrl = static_cast<RWindowCtrl*>(getFreeTree());

    if(glfwJoystickIsGamepad(jid))//断开的JID无法通过
    {
        gamepads.insert(RInputEvent::toJoystickID(jid));
        RjoystickPresentEvent e(RInputEvent::toJoystickID(jid), isConnected);
        wctrl->dispatchEvent(&e);
    }
    else if(gamepads.find(J) != gamepads.end())
    {
        gamepads.erase(RInputEvent::toJoystickID(jid));
        RjoystickPresentEvent e(RInputEvent::toJoystickID(jid), isConnected);
        wctrl->dispatchEvent(&e);
    }
    //不是手柄的JID忽视
}
void RWindowCtrl::resizeCallback(GLFWwindow *window, int width, int height)
{
    RWindowCtrl *wctrl = getWindowUserCtrl(window);

    if(wctrl->viewportPattern == FullWindow)
    {
        wctrl->width_ = width;
        wctrl->height_ = height;
        glViewport(0, 0, width, height);
        RResizeEvent e(width, height);
        wctrl->dispatchEvent(&e);
    }
    else if(wctrl->viewportPattern == KeepScale)
    {
        double ratio = static_cast<double>(width) / height;
        int newW = width;
        int newH = height;
        if(ratio > wctrl->viewportRatio_)
        {
            newW = height * wctrl->viewportRatio_;
            glViewport((width - newW) / 2.0, 0, newW, newH);
        }
        else
        {
            newH = width / wctrl->viewportRatio_;
            glViewport(0, (height - newH) / 2.0, newW, newH);
        }
        wctrl->width_ = newW;
        wctrl->height_ = newH;
        RResizeEvent e(newW, newH);
        wctrl->dispatchEvent(&e);
    }
}

void RWindowCtrl::mouseMoveCallback(GLFWwindow *window, double xpos, double ypos)
{
    RWindowCtrl *wctrl = getWindowUserCtrl(window);
    wctrl->inputs.updateMouseInput(RInputEvent::Mouse_None, RPoint(xpos, ypos));
}

void RWindowCtrl::keyboardCollback(GLFWwindow *window, int key, int , int action, int )
{
    RWindowCtrl *wctrl = getWindowUserCtrl(window);
    wctrl->inputs.updateKeyboardInput(RInputEvent::toKeyboards(key), RInputEvent::toButtonAction(action));
}

void RWindowCtrl::mouseButtonCallback(GLFWwindow *window, int button, int action, int )
{
    RWindowCtrl *wctrl = getWindowUserCtrl(window);
    RPoint p;
    if(action != RInputEvent::RELEASE)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        p.setPoint(x, y);
    }
    wctrl->inputs.updateMouseInput(RInputEvent::toMouseButtons(button), p);
}

void RWindowCtrl::mouseScrollCallback(GLFWwindow *window, double x, double y)
{
    RWindowCtrl *wctrl = getWindowUserCtrl(window);
    wctrl->scrolled.emit(y);
}
