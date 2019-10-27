#ifndef REVENT_H
#define REVENT_H

#include <string>

//通知事件 ************************

struct RjoystickPresentEvent
{
};

struct RUpdataEvent
{
};

struct RInitEvent
{
};

struct REnteredTreeEvent
{
    const std::string &treeNodeName;
};

struct RExitedTreeEvent
{
    const std::string &treeNodeName;
};

struct RResizeEvent
{
};

#endif // REVENT_H
