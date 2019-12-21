#include <RWindowCtrl.h>
#include <RJoiningThread.h>
#include "Testctr.h"

#include <RDebug.h>

int main(void)
{
    RWindowCtrl window;
    window.showWindow();
    TestCtr t("TestCtrl", &window);

    return window.exec();
}
