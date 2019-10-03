#include <RWindow.h>
#include "testctrl.h"
#include <RDebug.h>
#include <iostream>

int main()
{
    RWindow window;
    window.setWindowSize(960, 540);

    TestCtrl ctrl;

    return window.exec(&ctrl);
}
