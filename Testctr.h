#ifndef TESTCTR_H
#define TESTCTR_H

#include <RController.h>

class TestCtr : public RController
{
public:
    TestCtr();

    void contrl() override;

protected:
    void exitedTreeEvent(RExitedTreeEvent *event) override;
    void enteredTreeEvent(REnteredTreeEvent *event) override;
};

#endif // TESTCTR_H
