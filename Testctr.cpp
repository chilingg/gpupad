#include "Testctr.h"
#include <RDebug.h>

TestCtr::TestCtr():
    RController()
{

}

void TestCtr::contrl()
{

}

void TestCtr::exitedTreeEvent(RExitedTreeEvent *event)
{
    //RDebug() << "Exited tree with the " << event->treeNodeName << " of the " << getName();
}

void TestCtr::enteredTreeEvent(REnteredTreeEvent *event)
{
    //RDebug() << "Entered tree with the " << event->treeNodeName << " of the " << getName();
}
