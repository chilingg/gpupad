#include "Testctr.h"

#include <RWindowCtrl.h>
#include <RDebug.h>

int main()
{
    RWindowCtrl *w2;
    {
    RWindowCtrl window;
    w2 = new RWindowCtrl("W2");
    w2->changeParent(&window);
    w2->setWindowTitle("W2");
    TestCtr ctrl("Test", &window);
    window.exec();
    }
    delete w2;
    return 0;
}
