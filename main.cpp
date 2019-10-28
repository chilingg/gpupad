#include "Testctr.h"
#include <RDebug.h>

int main()
{
    TestCtr ts0, ts1;
    RDebug() << ts0.getPathName() << '\t' << ts1.getPathName();
    ts0.addChild(&ts1);
    RDebug() << ts0.getPathName() << '\t' << ts1.getPathName();
    ts0.freeAllChild();
    RDebug() << ts0.getPathName() << '\t' << ts1.getPathName();
    return 0;
}
