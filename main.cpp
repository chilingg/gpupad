#include "RWindow.h"
#include "RController.h"

int main()
{
    RWindow window;
    RController ctrl;

    window.setRootController(&ctrl);
    if(!window.initialize())
        return -1;
    window.exec();

    return 0;
}
