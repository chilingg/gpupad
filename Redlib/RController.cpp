#include "RController.h"

#include "RDebug.h"

const std::string RController::FREE_TREE_NAME = "_FreeTree_";
std::set<RInputRegistry::JoystickID> RController::gamepads;

RController::RController(const std::string &name, RController *parent):
    poolEvent([](){})
{
    //所有未指定父节点且名非FREE_TREE_NAME的，都挂在自由树下
    if(parent == nullptr && name != FREE_TREE_NAME)
        parent = getFreeTree();
    //排除FreeTree
    if(parent != nullptr)
    {
        parent->children_.push_back(this);
        state_ = parent->state_;
    }
    parent_ = parent;
    rename(name);
    //RDebug() << "Node is Created: " << getPathName();
}

RController::~RController()
{
    //RDebug() << "Delete with the " << name_ << " Children size: " << children_.size();
    if(parent_) parent_->deleteChild(this);
    while(!children_.empty())
    {
        auto c = children_.front();
#ifdef R_DEBUG
        if(this == getFreeTree())
        {
            printError("Unfree node in FreeTree: " + c->name_);
            c->parentToNull();
        }
        else
#endif
            //若FreeTree析构之时有仍有子结点存活，则死循
            c->changeParent(getFreeTree());
    }
}

RController *RController::getFreeTree()
{
    static RController freeTree(FREE_TREE_NAME);
    return &freeTree;
}

void RController::addChild(RController *child)
{
    if(child == nullptr)
    {
#ifdef R_DEBUG
        printError("Add null in " + getPathName());
#endif
        return;
    }
    if(child == this || isAncestor(child))
    {
#ifdef R_DEBUG
        printError("Add " + child->getPathName() + " in " + this->getPathName());
#endif
        return;
    }
    child->changeParent(this);
}

void RController::freeChild(RController *child)
{
    for(auto c : children_)
    {
        if(c == child)
        {
            child->changeParent(getFreeTree());
            break;
        }
    }
}

void RController::freeAllChild()
{
    std::list<RController*> empty;
    while(!children_.empty())
    {
        auto c = children_.front();
        c->changeParent(getFreeTree());
    }
    children_.swap(empty);
}

void RController::deleteChild(RController *child)
{
    for(auto c = children_.begin(); c != children_.end(); ++c)
    {
        if(*c == child)
        {
            children_.erase(c);
            child->parent_ = nullptr;
            break;
        }
    }
}

bool RController::isChild(RController *child) const
{
    //预想中第一代同辈子结点不是很多，so 手写
    for(auto c : children_)
    {
        if(child == c)
            return true;
    }
    return false;
}

bool RController::isAncestor(RController *node) const
{
    bool b = false;
    RController *parent = parent_;
    while(parent)
    {
        if(parent == node)
        {
            b = true;
            break;
        }
        parent = parent->parent_;
    }
    return b;
}

bool RController::isFree() const
{
    return parent_ == getFreeTree();
}

const std::string &RController::name() const
{
    return name_;
}

std::string RController::getPathName() const
{
    std::string name(name_);
    RController *parent = parent_;
    while(parent != nullptr)
    {
        name = parent->name_ + '/' + name;
        parent = parent->parent_;
    }
    return name;
}

std::string RController::getDefaultName() const
{
    return "Controller";
}

int RController::getChildrenSize() const
{
    return static_cast<int>(children_.size());
}

RController *RController::getParent()
{
    return parent_;
}

const std::list<RController *> &RController::getChildren() const
{
    return children_;
}

void RController::changeParent(RController *parent)
{
    if(parent_ == parent)
    {
#ifdef R_DEBUG
        printError("Changed parent" + parent_->getPathName() + " as " + parent->getPathName());
#endif
        return;
    }

    if(parent == nullptr)
        parent = getFreeTree();

    if(parent_)
    {
        //从父节点的子结点集中删除自己
        for(auto c = parent_->children_.begin(); c != parent_->children_.end(); ++c)
        {
            if(*c == this)
            {
                parent_->children_.erase(c);
                break;
            }
        }
        RExitedTreeEvent e(parent_);
        dispatchEvent(&e);
    }

    parent_ = parent;
    rename(name_);
    parent_->children_.push_back(this);

    REnteredTreeEvent e(this);
    dispatchEvent(&e);
    if(state_ != parent_->state_)
    {
        RInitEvent initEvent(parent);
        RCloseEvent closeEvent(parent);
        parent_->state_ == Looping ? this->dispatchEvent(&initEvent) : this->dispatchEvent(&closeEvent);
    }
}

void RController::rename(std::string name)
{
    //RDebug() << name_ << " to " << name;
    int num = 0;
    if(name.empty())
        name = getDefaultName();

    if(parent_)
    {
        for(auto sibling = parent_->children_.cbegin(); sibling != parent_->children_.cend(); ++sibling)
        {
            if(name == (*sibling)->name_ && this != *sibling)
            {
                name = name + std::to_string(num++);
                sibling = parent_->children_.cbegin();
            }
        }
    }

    name_.swap(name);
}

int RController::exec()
{
    assert(state_ != Looping);
    RInitEvent initEvent(this);
    dispatchEvent(&initEvent);
    while(state_ == Looping)
    {
        //需要的子类负责此函数指针的赋值
        poolEvent();
        control();
    }
    RCloseEvent closeEvent(this);
    dispatchEvent(&closeEvent);

    printError(state_ == Failure, "The Loop has unexpectedly finished");
    return state_;
}

void RController::breakLoop()
{
    if(state_ == Looping) state_ = Success;
}

void RController::allChildrenActive()
{
    auto child = children_.begin();
    while(child != children_.end())
    {
        if((*child)->isLooped())
        {
            (*child)->control();
            ++child;
        }
        else {
            freeChild(*child++);
        }
    }
}

void RController::terminateFreeTree()
{
    RController *freeTree = getFreeTree();
    if(freeTree->state_ == Failure)
        return;
    freeTree->state_ = Failure;
    RCloseEvent e(freeTree);
    freeTree->dispatchEvent(&e);
}

bool RController::isLooped() const
{
    return state_ == Looping;
}

void RController::control()
{
    allChildrenActive();
}

void RController::inputEvent(const RInputRegistry *)
{

}

void RController::joystickPresentEvent(RjoystickPresentEvent *)
{

}

void RController::initEvent(RInitEvent *)
{

}

void RController::closeEvent(RCloseEvent *)
{

}

void RController::enteredTreeEvent(REnteredTreeEvent *)
{

}

void RController::exitedTreeEvent(RExitedTreeEvent *)
{

}

RResizeEvent* RController::eventFilter(RResizeEvent *event)
{
    return event;
}

const RInputRegistry* RController::eventFilter(const RInputRegistry *event)
{
    return event;
}

void RController::resizeEvent(RResizeEvent *)
{

}

void RController::scrollEvent(RScrollEvent *)
{

}

void RController::dispatchEvent(const RInputRegistry *event)
{
    if(!(event = eventFilter(event)))
        return;

    for(auto child : children_)
    {
        child->dispatchEvent(event);
    }
    inputEvent(event);
}

void RController::dispatchEvent(RjoystickPresentEvent *event)
{
    for(auto child : children_)
    {
        child->dispatchEvent(event);
    }
    joystickPresentEvent(event);
}

void RController::dispatchEvent(RInitEvent *event)
{
    assert(state_ != Looping);
    if(state_ == Failure) return;//错误状态无法进入循环

    state_ = Looping;
    for(auto child : children_)
    {
        child->dispatchEvent(event);
    }
    initEvent(event);
}

void RController::dispatchEvent(RCloseEvent *event)
{
    assert(state_ != Looping);
    for(auto child : children_)
    {
        child->state_ = state_;
        child->dispatchEvent(event);
    }
    closeEvent(event);
}

void RController::dispatchEvent(REnteredTreeEvent *event)
{
    for(auto child : children_)
    {
        child->dispatchEvent(event);
    }
    enteredTreeEvent(event);
    treeEntered.emit();//入树信号
}

void RController::dispatchEvent(RExitedTreeEvent *event)
{
    for(auto child : children_)
    {
        child->dispatchEvent(event);
    }
    exitedTreeEvent(event);
    treeExited.emit();//出树信号
}

void RController::parentToNull()
{
#ifdef R_DEBUG
    if(!parent_)
    {
        printError("Null parent to null!");
        return;
    }
#endif

    //从父节点的子结点集中删除自己
    for(auto c = parent_->children_.begin(); c != parent_->children_.end(); ++c)
    {
        if(*c == this)
        {
            parent_->children_.erase(c);
            break;
        }
    }
    parent_ = nullptr;
}

void RController::dispatchEvent(RResizeEvent *event)
{
    if(!(event = eventFilter(event)))
        return;

    for(auto child : children_)
    {
        child->dispatchEvent(event);
    }
    resizeEvent(event);
}

void RController::dispatchEvent(RScrollEvent *event)
{
    for(auto child : children_)
    {
        child->dispatchEvent(event);
    }
    scrollEvent(event);
}
