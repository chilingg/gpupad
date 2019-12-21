#ifndef RCONTRLLER_H
#define RCONTRLLER_H

#include "RNotifyEvent.h"
#include "RSigslot.h"

#include <string>
#include <list>

class RController: public RSigslot
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
    explicit RController(const std::string &name = "Controller", RController *parent = nullptr);
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
    const std::string& name() const;
    std::string getPathName() const;
    int getChildrenSize() const;
    RController* getParent();
    const std::list<RController*>& getChildren() const;
    RController *getTreeNode(const std::string &path);

    //执行函数
    int exec();//调用allAction()循环调用子孙节点的control()
    void breakLoop();//退出exec循环

protected:
    virtual std::string getDefaultName() const;//建议子类重写

    //事件响应。感兴趣的子类负责重写
    virtual void inputEvent(RInputEvent *event);
    virtual void resizeEvent(RResizeEvent *event);
    virtual void scrollEvent(RScrollEvent *event);
    virtual void initEvent(RInitEvent *event);//exec循环开始
    virtual void closeEvent(RCloseEvent *event);//exec循环终止
    virtual void enteredTreeEvent(REnteredTreeEvent *event);
    virtual void exitedTreeEvent(RExitedTreeEvent *event);

    //事件截断。当前实例发布事件前调用，返回的事件传递给子结点发布，若返回nullptr则截断当前事件的发布
    virtual RResizeEvent* eventFilter(RResizeEvent *event);
    virtual RInputEvent *eventFilter(RInputEvent *event);

    //事件发布接口 PS:深度优先、由下至上
    //由子类负责调用
    void dispatchEvent(RInputEvent *event);
    void dispatchEvent(RResizeEvent *event);
    void dispatchEvent(RScrollEvent *event);
    //已由RController负责调用
    void dispatchEvent(RInitEvent *event);
    void dispatchEvent(RCloseEvent *event);
    void dispatchEvent(REnteredTreeEvent *event);
    void dispatchEvent(RExitedTreeEvent *event);

    void allChildrenActive();//调用所有子节点的contral()

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
