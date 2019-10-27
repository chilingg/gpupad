/*
 * sigslot优点
 * 不用担心空回调，当回调对象析构时会自动disconnect
 * 支持多线程，线程安全，有锁
 *
 * sigslot缺点
 * 只能回调void类型函数，不支持返回值
 * slot没有优先级，不能动态调整回调队列中的先后顺序
 ×
 * slot函数（被回调的函数）就是普通的成员函数，但有以下限制：
 * 返回值必须为void
 * slot参数个数范围为0-8个
 * 实现slot的类必须继承自has_slots<>
 *
 * Sigslot库用法
 * signal0<> sig1;//没有参数的信号
 * signal2<char *, int> sig2;//发送两个参数
 * sig.sg1.conncent(&slt, &mySlot::on_func1);//连接信号
 * sig.sg1.disconnect(&slt);//断开信号
 * sig.sg1.disconnect_all();//断开全部信号
 * sig.sg1();//发送信号
 * sig.sg1.emit();
 × sig.sg2("str",0.1);
*/

#ifndef RCONTRLLER_H
#define RCONTRLLER_H

#include "Extern/sigslot.h"
#include "RNotifyEvent.h"
#include "RInputEvent.h"

#include <string>
#include <list>

class RController: public sigslot::has_slots<>
{
public:
    RController(const std::string &name = "", RController *parent = nullptr);
    ~RController();

    static RController *getFreeTree();

    void addChild(RController *child);
    void deleteChild(RController *child);
    void deleteAllChild();
    bool isChild(RController *child) const;
    void changeParent(RController *parent);
    void rename(std::string name);
    void exec();
    void inactive();
    bool isActive();

protected:
    //事件响应
    virtual void contrl() = 0;
    virtual void inputEvent(RInputEvent *event);
    virtual void joystickPresentEvent(RjoystickPresentEvent *event);
    virtual void updataEvent(RUpdataEvent *event);
    virtual void initEvent(RInitEvent *event);
    virtual void enteredTreeEvent(REnteredTreeEvent *event);
    virtual void exitedTreeEvent(RExitedTreeEvent *event);
    virtual void resizeEvent(RResizeEvent *event);
    //事件发布接口
    void allAction();
    void createInputEventToDspt();
    void createJoystickPresentEventToDspt();
    void createUpdataEventToDspt();
    void createInitEventToDspt();
    void createEnteredTreeEventToDspt(const std::string &name);
    void createExitedTreeEventToDspt(const std::string &name);
    void createResizeEventToDspt();

    //信号
    sigslot::signal0<> treeEntered;
    sigslot::signal0<> treeExited;

private:
    static const std::string FREE_TREE_NAME;
    //分发事件 PS:深度优先、由下至上
    void dispatchEvent(RInputEvent *event);
    void dispatchEvent(RjoystickPresentEvent *event);
    void dispatchEvent(RUpdataEvent *event);
    void dispatchEvent(RInitEvent *event);
    void dispatchEvent(REnteredTreeEvent *event);
    void dispatchEvent(RExitedTreeEvent *event);
    void dispatchEvent(RResizeEvent *event);

    std::string name_;
    std::list<RController*> children_;
    RController *parent_ = nullptr;
    bool active_ = true;
};

#endif // RCONTRLLER_H
