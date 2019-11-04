#include "Testctr.h"

#include <RWindowCtrl.h>
#include <RDebug.h>

int main()
{
    RWindowCtrl window;
    window.enableGamepad();
    TestCtr ctrl("Test", &window);
    return window.exec();
}
