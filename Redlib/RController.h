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
    enum Status {
        Success = EXIT_SUCCESS,
        Failure = EXIT_FAILURE,
        Looping,
    };
    static RController *getFreeTree();
    static void terminateFreeTree();

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
    bool isLooped() const;
    bool isChild(RController *child) const;
    bool isAncestor(RController *node) const;//祖辈
    bool isFree() const;
    const std::string& getName() const;
    std::string getPathName() const;
    int getChildrenSize() const;
    RController* getParent();
    //执行函数
    int exec();//调用allAction()循环调用子孙节点的control()
    void breakLoop();//退出exec循环

protected:
    using Signal0 = sigslot::signal0<>;
    template<typename T1>
    using Signal1 = sigslot::signal1<T1>;
    template<typename T1, typename T2>
    using Signal2 = sigslot::signal2<T1, T2>;
    template<typename T1, typename T2, typename T3>
    using Signal3 = sigslot::signal3<T1, T2, T3>;
    template<typename T1, typename T2, typename T3, typename T4>
    using Signal4 = sigslot::signal4<T1, T2, T3, T4>;
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    using Signal5 = sigslot::signal5<T1, T2, T3, T4, T5>;
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    using Signal6 = sigslot::signal6<T1, T2, T3, T4, T5, T6>;
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    using Signal7 = sigslot::signal7<T1, T2, T3, T4, T5, T6, T7>;
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    using Signal8 = sigslot::signal8<T1, T2, T3, T4, T5, T6, T7, T8>;

    static std::set<RInputEvent::JoystickID> gamepads;

    virtual std::string getDefaultName() const;//建议子类重写
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

    void allChildrenActive();//调用所有子节点的contral()

    RInputEvent inputs;
    void (*poolEvent)();//exec循环中调用的一个零参无返函数

    //信号
    Signal0 treeEntered;
    Signal0 treeExited;

private:
    static const std::string FREE_TREE_NAME;

    void parentToNull();
    void deleteChild(RController *child);

    std::string name_;
    std::list<RController*> children_;
    RController *parent_ = nullptr;
    Status state_ = Success;
};

#endif // RCONTRLLER_H
