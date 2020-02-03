#include <RDebug.h>
#include <RContext.h>

using namespace Redopera;

// 使用RWindow的场合，初始化与结束由RWindow实例负责

int main()
{
    if(check(!RContext::initialization(), "Failure initialization OpenGL context!"))
        exit(EXIT_FAILURE);

    std::cout << "OpenGl Version:" << GLVersion.major << '.' << GLVersion.minor << std::endl;

    RContext::terminate();
    return 0;
}
