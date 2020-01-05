#ifndef RCONTRLLER_H
#define RCONTRLLER_H

#include "RNotifyEvent.h"
#include "RSigslot.h"

#include <string>
#include <list>

class RController: public RSlot
{
public:
    struct TranslationInfo
    {
        const RController *sender;
        RSize size;
        RPoint pos = RPoint(0);
        RVector3 rotate { 0.0f, 0.0f, 0.0f};
    };

    enum Status {
        Normal = EXIT_SUCCESS,
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
    //变化信息传递，默认为原样转递给子结点
    virtual void translation(const TranslationInfo &info);

    //设置函数
    void addChild(RController *child);
    void freeChild(RController *child);
    void freeAllChild();
    void changeParent(RController *parent);
    void rename(std::string name);
    //查询函数
    Status status() const;
    bool isLooped() const;
    bool isNormal() const;
    bool isFailur() const;
    bool isChild(RController *child) const;
    bool isAncestor(RController *node) const;//祖辈
    bool isFree() const;
    const std::string& name() const;
    std::string getPathName() const;
    int getChildrenSize() const;
    RController* getParent();
    const std::list<RController*>& getChildren() const;
    RController *getTreeNode(const std::string &path);//改改改改改改改改改改改改改改改改改改改改改改改

    //执行函数
    int exec(); //调用自身及所有子节点的contral()
    void breakLoop();//退出exec循环

protected:
    virtual std::string getDefaultName() const;//建议子类重写

    //事件响应。感兴趣的子类负责重写
    virtual void inputEvent(RInputEvent *event);
    virtual void scrollEvent(RScrollEvent *event);
    virtual void startEvent(RStartEvent *event);//exec循环开始
    virtual void closeEvent(RCloseEvent *event);//尝试终止exec循环
    virtual void finishEvent(RFinishEvent *event);//exec循环终止
    virtual void enteredTreeEvent(REnteredTreeEvent *event);
    virtual void exitedTreeEvent(RExitedTreeEvent *event);

    //事件截断。当前实例发布事件前调用，返回的事件传递给子结点发布，若返回nullptr则截断当前事件的发布
    virtual RInputEvent *eventFilter(RInputEvent *event);

    //事件发布接口 PS:深度优先、由下至上
    //由子类负责调用
    void dispatchEvent(RInputEvent *event);
    void dispatchEvent(RScrollEvent *event);
    void dispatchEvent(RCloseEvent *event);
    //已由RController负责调用
    void dispatchEvent(RStartEvent *event);
    void dispatchEvent(RFinishEvent *event);
    void dispatchEvent(REnteredTreeEvent *event);
    void dispatchEvent(RExitedTreeEvent *event);

    void allChildrenActive(); //调用所有子节点的contral()

    //信号
    RSignal<> started;
    RSignal<> finished;
    RSignal<> treeEntered;
    RSignal<> treeExited;

private:
    static const std::string FREE_TREE_NAME;

    void parentToNull();
    void deleteChild(RController *child);

    std::string name_;
    std::list<RController*> children_;
    RController *parent_ = nullptr;
    Status state_ = Normal;

    _RSLOT_TAIL_
};

#endif // RCONTRLLER_H
