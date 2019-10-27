#include "RController.h"

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
    deleteAllChild();
}

RController *RController::getFreeTree()
{
    static RController freeTree(FREE_TREE_NAME);
    return &freeTree;
}

void RController::addChild(RController *child)
{
    if(child == this)
        return;
    child->changeParent(this);
}

void RController::deleteChild(RController *child)
{
    auto c = children_.begin();
    for(; c != children_.end(); ++c)
    {
        if(*c == child)
            break;
    }
    if(c != children_.end())
    {
        child->changeParent(getFreeTree());
    }
}

void RController::deleteAllChild()
{
    std::list<RController*> empty;
    for(auto c : children_)
    {
        c->changeParent(getFreeTree());
    }
    children_.swap(empty);
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

void RController::changeParent(RController *parent)
{
    if(parent_)
    {
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
    createEnteredTreeEventToDspt(parent_->name_);
}

void RController::rename(std::string name)
{
    int num = 1;
    if(name.empty())
        name = "Contrller" + std::to_string(num);

    if(parent_)
    {
        for(auto sibling = parent_->children_.cbegin(); sibling != parent_->children_.cend(); ++sibling)
        {
            if(name == (*sibling)->name_ && this != *sibling)
            {
                name = "Contrller" + std::to_string(++num);
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

bool RController::isActive()
{
    return active_;
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
    REnteredTreeEvent e{name};
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
