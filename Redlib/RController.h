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
    static RController *getFreeTree();

    //构造参数指定的parent不会触发入树事件，默认的节点名须与重写或继承的getDefaultName()版本一致
    RController(const std::string &name = "Controller", RController *parent = nullptr);
    RController(const RController &) = delete;
    RController& operator=(const RController &) = delete;
    virtual ~RController();

    //控制类最主要函数，需子类自己实现
    virtual void control();

    //设置函数
    void addChild(RController *child);
    void freeChild(RController *child);
    void freeAllChild();
    void changeParent(RController *parent);
    void rename(std::string name);
    //查询函数
    bool isLoop() const;
    bool isChild(RController *child) const;
    bool isAncestor(RController *node) const;//祖辈
    bool isFree() const;
    const std::string& getName() const;
    std::string getPathName() const;
    int getChildrenSize() const;
    //执行函数
    int exec();//调用allAction()循环调用子孙节点的control()
    void inactive();//退出exec循环
    void allChildrenActive();//调用所有子节点的contral()

protected:
    virtual std::string getDefaultName() const;//建议重写
    //事件响应。感兴趣的子类负责重写
    virtual void inputEvent(const RInputEvent *event);
    virtual void joystickPresentEvent(RjoystickPresentEvent *event);
    virtual void resizeEvent(RResizeEvent *event);
    virtual void initEvent(RInitEvent *event);//exec循环开始
    virtual void closeEvent(RCloseEvent *event);//exec循环终止
    virtual void enteredTreeEvent(REnteredTreeEvent *event);
    virtual void exitedTreeEvent(RExitedTreeEvent *event);

    //事件发布接口 PS:深度优先、由下至上
    //由子类负责调用
    void dispatchEvent(const RInputEvent *event);
    void dispatchEvent(RjoystickPresentEvent *event);
    void dispatchEvent(RResizeEvent *event);
    //已由RController负责调用
    void dispatchEvent(RInitEvent *event);
    void dispatchEvent(RCloseEvent *event);
    void dispatchEvent(REnteredTreeEvent *event);
    void dispatchEvent(RExitedTreeEvent *event);

    void parentToNull();
    void deleteChild(RController *child);

    //信号
    sigslot::signal0<> treeEntered;
    sigslot::signal0<> treeExited;

private:
    static const std::string FREE_TREE_NAME;

    std::string name_;
    std::list<RController*> children_;
    RController *parent_ = nullptr;
    bool loop_ = false;
};

#endif // RCONTRLLER_H
