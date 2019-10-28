#include "RController.h"

#include "RDebug.h"

const std::string RController::FREE_TREE_NAME = "_FreeTree_";

RController::RController(const std::string &name, RController *parent):
    name_(),
    children_()
{
    //所有未指定父节点且名非FREE_TREE_NAME的，都挂在自由树下
    if(parent == nullptr && name != FREE_TREE_NAME)
        parent = getFreeTree();

    rename(name);
    if(parent != nullptr)
        parent->addChild(this);
}

RController::~RController()
{
    //RDebug() << "Delete with the " << name_ << ". Children size: " << children_.size();
    if(parent_)
        parent_->deleteChild(this);
    freeAllChild();
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
    auto c = children_.begin();
    for(; c != children_.end(); ++c)
    {
        if(*c == child)
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
        auto c = children_.back();
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
            //RDebug() << "Deleting " << child->name_ << " in " << name_;
            children_.erase(c);
            break;
        }
    }
    child->parent_ = nullptr;
    child->createExitedTreeEventToDspt(name_);
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

int RController::getChildrenSize() const
{
    return static_cast<int>(children_.size());
}

void RController::changeParent(RController *parent)
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
        createExitedTreeEventToDspt(parent_->name_);
    }

    if(!parent)
        parent = getFreeTree();

    parent_ = parent;
    rename(name_);
    parent_->children_.push_back(this);

    createEnteredTreeEventToDspt(parent_->name_);
}

void RController::rename(std::string name)
{
    //RDebug() << name_ << " to " << name;
    int num = 0;
    if(name.empty())
        name = "Contrller";

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

void RController::exec()
{
    active_ = true;

    while(active_)
    {
        allAction();
    }
}

void RController::inactive()
{
    active_ = false;
}

bool RController::isActive() const
{
    return active_;
}

void RController::contrl()
{

}

void RController::inputEvent(RInputEvent *)
{

}

void RController::joystickPresentEvent(RjoystickPresentEvent *)
{

}

void RController::updataEvent(RUpdataEvent *)
{

}

void RController::initEvent(RInitEvent *)
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

void RController::allAction()
{
    for(auto child : children_)
    {
        child->contrl();
    }
    contrl();
}

void RController::createEnteredTreeEventToDspt(const std::string &name)
{
    REnteredTreeEvent e{name};
    dispatchEvent(&e);
}

void RController::createExitedTreeEventToDspt(const std::string &name)
{
    RExitedTreeEvent e{name};
    dispatchEvent(&e);
}

void RController::dispatchEvent(RInputEvent *event)
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

void RController::dispatchEvent(RUpdataEvent *event)
{
    for(auto child : children_)
    {
        child->dispatchEvent(event);
    }
    updataEvent(event);
}

void RController::dispatchEvent(RInitEvent *event)
{
    for(auto child : children_)
    {
        child->dispatchEvent(event);
    }
    initEvent(event);
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

void RController::dispatchEvent(RResizeEvent *event)
{
    for(auto child : children_)
    {
        child->dispatchEvent(event);
    }
    resizeEvent(event);
}
