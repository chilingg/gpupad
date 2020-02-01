#include <RDebug.h>
#include <RController.h>

using namespace Redopera;

class TestCtl : public RController
{
protected:
    void control() override
    {
        char c;
        rDebug << "循环中是否继续? y/n";
        std::cin >> c;
        if(c == 'n')
            breakLoop();
    }
    void closeEvent(RCloseEvent &event) override
    {
        char c;
        rDebug << "是否退出? y/n";
        std::cin >> c;
        if(c == 'n')
            event.stop = true;
    }
};

int main()
{
    TestCtl t;

    return t.exec();
}
