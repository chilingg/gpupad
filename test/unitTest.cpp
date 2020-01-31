#include <RDebug.h>
#include <RThread.h>
#include <RController.h>

using namespace Redopera;

class TestCrl : public RController
{
public:
    explicit TestCrl(RController *parent, const std::string &name):
        RController(parent, name)
    {}

    ~TestCrl() override = default;

protected:
    void startEvent(RStartEvent &) override
    {
        rDebug << std::this_thread::get_id() << ": start in " << getPathName();
    }
    void closeEvent(RCloseEvent &) override
    {
        rDebug << std::this_thread::get_id() << ": close in " << getPathName();
    }
    void finishEvent(RFinishEvent &) override
    {
        rDebug << std::this_thread::get_id() << ": finish in " << getPathName();
    }
};

void test()
{
    TestCrl c1(nullptr, "c1");
    TestCrl c2(&c1, "c2");
    TestCrl c3(&c2, "c3");
    TestCrl c4(&c2, "c4");
    c1.addChild(&c4);
    c2.changeParent(&c4);

    c1.exec();
}

int main()
{
    RThread thread(test);
    RThread thread2(test);

    char c;
    RController::TreeList list;
    do {
        std::cin >> c;
        if(c == 'q')
        {
            RController::RootTree root = RController::rootTree();
            if(root->empty())
                break;
            root->begin()->second->breakLoop();
        }
        else if(c == 'l')
        {
            RController::queryTreeList(list);
            for(auto &node : list)
                rDebug << node.second;
        }
    } while(1);

    return 0;
}
