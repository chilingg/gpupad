#include "RController.h"

#include "RDebug.h"

const std::string RController::FREE_TREE_NAME = "_FreeTree_";
std::set<RInputEvent::JoystickID> RController::gamepads;

RController::RController(const std::string &name, RController *parent)
{
    //所有未指定父节点且名非FREE_TREE_NAME的，都挂在自由树下
    if(parent == nullptr && name != FREE_TREE_NAME)
        parent = getFreeTree();

    if(parent != nullptr)
        parent->children_.push_back(this);
    parent_ = parent;
    rename(name);
}

RController::~RController()
{
    //RDebug() << "Delete with the " << name_ << " Children size: " << children_.size();
    if(parent_)
        parent_->deleteChild(this);
    while(!children_.empty())
    {
        auto c = children_.front();
        c->parentToNull();
    }
}

RController *RController::getFreeTree()
{
    static RController freeTree(FREE_TREE_NAME);
    return &freeTree;
}

void RController::addChild(RController *child)
{
    if(child == this || isAncestor(child))
        return;
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

const std::string &RController::getName() const
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

void RController::changeParent(RController *parent)
{
    if(parent_ == parent)
        return;

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

    if(!parent)
        parent = getFreeTree();

    parent_ = parent;
    rename(name_);
    parent_->children_.push_back(this);

    REnteredTreeEvent e(this);
    dispatchEvent(&e);
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
    loop_ = true;

    RInitEvent initEvent(this);
    dispatchEvent(&initEvent);
    while(loop_)
    {
        control();
    }
    RCloseEvent closeEvent(this);
    dispatchEvent(&closeEvent);

    return EXIT_SUCCESS;
}

void RController::inactive()
{
    loop_ = false;
}

void RController::allChildrenActive()
{
    for(auto child : children_)
        child->control();
}

bool RController::isLoop() const
{
    return loop_;
}

void RController::control()
{
    allChildrenActive();
}

void RController::inputEvent(const RInputEvent *)
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

void RController::resizeEvent(RResizeEvent *)
{

}

void RController::dispatchEvent(const RInputEvent *event)
{
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
    for(auto child : children_)
    {
        child->dispatchEvent(event);
    }
    initEvent(event);
}

void RController::dispatchEvent(RCloseEvent *event)
{
    for(auto child : children_)
    {
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
        parent_ = nullptr;
    }
}

void RController::dispatchEvent(RResizeEvent *event)
{
    for(auto child : children_)
    {
        child->dispatchEvent(event);
    }
    resizeEvent(event);
}
