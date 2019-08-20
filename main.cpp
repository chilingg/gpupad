#include <RWindow.h>
#include "testctrl.h"

int main()
{
    RWindow window;
    TestCtrl ctrl;

    window.setRootController(&ctrl);

    if(!window.initialize())
        return -1;

    return window.exec();
}
