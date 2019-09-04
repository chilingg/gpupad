#include <RWindow.h>
#include "testctrl.h"

int main()
{
    RWindow window;
    window.setWindowSize(960, 540);
    if(!window.initialize())
        return -1;

    TestCtrl ctrl;
    window.setRootController(&ctrl);

    return window.exec();
}
