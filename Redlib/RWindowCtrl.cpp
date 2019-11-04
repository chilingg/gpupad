#include "RWindowCtrl.h"
#include "RResource/RResource.h"

#include "RDebug.h"

std::vector<RWindowCtrl*> RWindowCtrl::windowCtrls_;
int RWindowCtrl::count = 0;
bool RWindowCtrl::gamepadModule_ = false;
bool RWindowCtrl::vSync_ = true;
std::set<RInputEvent::JoystickID> RWindowCtrl::gamepads;

RWindowCtrl::RWindowCtrl(const std::string &name, RController *parent):
    RController(name, parent),
    window_(nullptr)
{
    if(++count == 1)
    {
        if(!glfwInit())//初始化GLFW
        {
            printError("Failed to initialize GLFW");
            parentToNull();
            exit(EXIT_FAILURE);
        }
        if(gamepadModule_)
            enableGamepad();
    }

    glfwDefaultWindowHints();
    //glfwWindowHint(GLFW_DECORATED, GL_FALSE);//取消边框与标题栏
    window_ = glfwCreateWindow(width_, height_, "Redopera", nullptr, nullptr);
    if(!window_)
    {
        printError("Fainled to create GLFW window!");
        parentToNull();
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    windowCtrls_.push_back(this);
    if(glfwGetCurrentContext())
    {
        printError("Creating multiple windows on the same thread is not allowed!");
        parentToNull();
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    //GLFW将该context设置为当前线程主context
    glfwMakeContextCurrent(window_);
    hideWindow();

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

    //垂直同步，参数为1锁60帧
    glfwSwapInterval(vSync_);
    glfwSetErrorCallback(errorCallback);//错误回调
    glfwSetFramebufferSizeCallback(window_, resizeCallback);
    glfwSetKeyCallback(window_, keyboardCollback);
    glfwSetMouseButtonCallback(window_, mouseButtonCallback);
    glfwSetScrollCallback(window_, mouseWheelCallback);

    //设置混合函数 Ps:混合需另外开启
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

RWindowCtrl::~RWindowCtrl()
{
    assert(window_);
    for(auto w = windowCtrls_.begin(); w != windowCtrls_.end(); ++w)
    {
        if(*w == this)
        {
            windowCtrls_.erase(w);
            break;
        }
    }
    glfwDestroyWindow(window_);

    if(--count == 0)
        glfwTerminate();
}

void RWindowCtrl::control()
{
    //清除颜色缓存
    glClearBufferfv(GL_COLOR, 0, backgroundColor);

    //GLFW事件触发
    glfwPollEvents();
    //更新手柄输入
    for(auto jid : gamepads)
        inputEvent.updateGamepadButtonInput(jid);
    //发布输入事件
    dispatchEvent(&inputEvent);
    //调动子结点控制
    allChildrenActive();

    glfwSwapBuffers(window_);

    if(glfwWindowShouldClose(window_))
        inactive();
}

void RWindowCtrl::setWindowSize(int width, int height)
{
    glfwSetWindowSize(window_, width, height);
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
    vSync_ = enable ? 1 : 0;
    glfwSwapInterval(vSync_);
}

void RWindowCtrl::closeWindow()
{
    glfwSetWindowShouldClose(window_, GLFW_TRUE);
}

void RWindowCtrl::enableGamepad()
{
    gamepadModule_ = true;

    if(count > 0)
    {
        //加载手柄映射
        if(updataGamepadMappings(":/data/gamecontrollerdb.txt"))
        {
            std::string mappingCode = std::string() + RInputEvent::gamepadMappingCode0
                    + RInputEvent::gamepadMappingCode1 + RInputEvent::gamepadMappingCode2;
            glfwUpdateGamepadMappings(mappingCode.c_str());
        }
        glfwSetJoystickCallback(joystickPresentCallback);

        //需手动检测一次手柄连接，检测之前的连接
        for(int i = RInputEvent::joystick1; i <= RInputEvent::joystickMaxNum; ++i)
        {
            if(glfwJoystickIsGamepad(i))
                gamepads.insert(RInputEvent::toJoystickID(i));
        }
    }
}

void RWindowCtrl::disableGamepad()
{
    if(!gamepadModule_)
        return;

    if(!windowCtrls_.empty())
    {
        for(int i = RInputEvent::joystick1; i <= RInputEvent::joystickMaxNum; ++i)
        {
            if(glfwJoystickIsGamepad(i))
            {
                joystickPresentCallback(i, RInputEvent::joystickDisconnected);
            }
        }
        gamepadModule_ = false;
        glfwSetJoystickCallback([](int , int){});
    }
}

bool RWindowCtrl::updataGamepadMappings(std::string path)
{
    try{
        std::string mappingCode = RResource::getTextFileContent(path);
        glfwUpdateGamepadMappings(mappingCode.c_str());
    }
    catch(...)
    {
        printError("Failed to updata gamepad mapping! In path: " + path + '\n' +
                   "To https://github.com/gabomdq/SDL_GameControllerDB download gamecontrollerdb.txt file.");
        return false;
    }
    return true;
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
    showWindow();
    for(int i = RInputEvent::joystick1; i <= RInputEvent::joystickMaxNum; ++i)
    {
        if(glfwJoystickIsGamepad(i))
        {
            RjoystickPresentEvent e(RInputEvent::toJoystickID(i), true);
            dispatchEvent(&e);
        }
    }
}

void RWindowCtrl::closeEvent(RCloseEvent *)
{
    hideWindow();
}

void RWindowCtrl::errorCallback(int error, const char *description)
{
    printError(std::to_string(error) + '\n' + description);
}

void RWindowCtrl::resizeCallback(GLFWwindow *window, int width, int height)
{
    RWindowCtrl *wctrl = getWindowCtrl(window);

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
        double ratio = wctrl->width_ / wctrl->height_;
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
    RWindowCtrl *wctrl = getWindowCtrl(window);
    wctrl->inputEvent.updateMouseInput(RInputEvent::Mouse_None, RPoint(xpos, ypos));
}

void RWindowCtrl::keyboardCollback(GLFWwindow *window, int key, int , int action, int )
{
    RWindowCtrl *wctrl = getWindowCtrl(window);
    wctrl->inputEvent.updateKeyboardInput(RInputEvent::toKeyboards(key), RInputEvent::toButtonAction(action));
}

void RWindowCtrl::mouseButtonCallback(GLFWwindow *window, int button, int action, int )
{
    RWindowCtrl *wctrl = getWindowCtrl(window);
    RPoint p;
    if(action != RInputEvent::RELEASE)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        p.setPoint(x, y);
    }
    wctrl->inputEvent.updateMouseInput(RInputEvent::toMouseButtons(button), p);
}

void RWindowCtrl::mouseWheelCallback(GLFWwindow *window, double x, double y)
{
    RWindowCtrl *wctrl = getWindowCtrl(window);
    RPoint p;
    p.setX(x > 0 ? 1 : -1);
    p.setY(y > 0 ? 1 : -1);
    wctrl->inputEvent.updateMouseInput(RInputEvent::Mouse_Wheel, p);
}

void RWindowCtrl::joystickPresentCallback(int jid, int event)
{
    RDebug() << "J present";
    bool isConnected = event == RInputEvent::joystickConnected ? true : false;
    if(glfwJoystickIsGamepad(jid))
    {
        if(isConnected)
            gamepads.insert(RInputEvent::toJoystickID(jid));
        else
            gamepads.insert(RInputEvent::toJoystickID(jid));
    }

    for(const auto wctrl : windowCtrls_)
    {
        RjoystickPresentEvent e(RInputEvent::toJoystickID(jid), isConnected);
        wctrl->dispatchEvent(&e);
    }
}

RWindowCtrl *RWindowCtrl::getWindowCtrl(const GLFWwindow *window)
{
#ifndef R_NO_DEBUG
    if(printError(!window, "No corresponding window found!"))
        exit(EXIT_FAILURE);
#endif

    for(const auto wctrl : windowCtrls_)
    {
        if(wctrl->window_ == window)
            return wctrl;
    }

    printError("No corresponding window found!");
    exit(EXIT_FAILURE);
}
