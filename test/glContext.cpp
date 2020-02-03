#include <RDebug.h>
#include <RContext.h>

using namespace Redopera;

// 使用RWindow的场合，初始化与结束由RWindow实例负责

int main()
{
    rDebug << "======== OpenGL Context 创建测试 ========\n";

    if(check(!RContext::initialization(), "Failure initialization OpenGL context!"))
        exit(EXIT_FAILURE);

    rDebug << "尝试创建OpenGL3.3版本Context...";
    RContext::ContexFormat format;
    format.versionMajor = 3;
    format.versionMinor = 3;

    if(RContext::setContexAsThisThread(format))
        std::cout << "OpenGl Version " << GLVersion.major << '.' << GLVersion.minor << " 创建成功" << std::endl;

    RContext::destroyContexAsThisThread();
    format.versionMajor = 4;
    rDebug << "\n尝试获取更高版本...";
    for(int i = 0; i <= 6; ++i)
    {
        format.versionMinor = i;

        if(RContext::setContexAsThisThread(format))
            std::cout << "OpenGl Version " << GLVersion.major << '.' << GLVersion.minor << " 创建成功" << std::endl;
        else
            break;
    }

    rDebug << "\n测试结束";
    RContext::terminate();

    return 0;
}
