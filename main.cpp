#include "Testctr.h"

#include <RWindowCtrl.h>
#include <RDebug.h>

int main()
{
    RWindowCtrl *w2;
    {
    RWindowCtrl window;
    //RWindowCtrl::WindowDecorate(false);
    w2 = new RWindowCtrl;
    w2->showWindow();
    TestCtr ctrl("Test", &window);
    window.exec();
    }
    delete w2;
    return 0;
}
