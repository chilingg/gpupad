#include "RInputEvent.h"

#include "RDebug.h"

RInputEvent::RInputEvent()
{
    //添加一个外部文件映射解析
    keyboardInputs_.push_back({KEY_SPACE, No_Modifier, "SPACE"});
    buttonPattern_["SPACE"] = false;
    keyboardInputs_.push_back({KEY_ESCAPE, No_Modifier, "ESC"});
    buttonPattern_["ESC"] = false;
    keyboardInputs_.push_back({KEY_UP, No_Modifier, "UP"});
    buttonPattern_["UP"] = false;
    keyboardInputs_.push_back({KEY_DOWN, No_Modifier, "DOWN"});
    buttonPattern_["DOWN"] = false;
    keyboardInputs_.push_back({KEY_RIGHT, No_Modifier, "RIGHT"});
    buttonPattern_["RIGHT"] = false;
    keyboardInputs_.push_back({KEY_LEFT, No_Modifier, "LEFT"});
    buttonPattern_["LEFT"] = false;
}

bool RInputEvent::keyboardInput(RInputEvent::Keyboard key,
                                     RInputEvent::ButtonAction action,
                                     RInputEvent::KeyboardModufuer modes)
{
    for(auto k : keyboardInputs_)
    {
        if(k.key_ == key && k.mods_ == modes)
        {
            buttonPattern_[k.inputPattern_] = action == RELEASE ? false : true;
            return true;
        }
    }
    return false;
}

void RInputEvent::addKeyboardPattern(const std::string &inputPattern, RInputEvent::Keyboard key,
                                     RInputEvent::KeyboardModufuer modes)
{
#ifndef R_NO_DEBUG
    for(auto k : keyboardInputs_)
    {
        if(k.key_ == key && k.mods_ == modes)
        {
            printError("Add duplicate keyboard pattern: " + inputPattern);
            exit(EXIT_FAILURE);
        }
    }
#endif
    keyboardInputs_.push_back({key, modes, inputPattern});
    buttonPattern_[inputPattern] = false;
}

bool RInputEvent::checkButtonPattern(const std::string &pattern) const
{
    auto p = buttonPattern_.find(pattern);
#ifndef R_NO_DEBUG
    if(printError(p == buttonPattern_.end(), "Access to nonexistent patterns: " + pattern))
        exit(EXIT_FAILURE);
#endif
    return p->second;
}

void RInputEvent::resetPattern()
{
    for(auto &p : buttonPattern_)
    {
        p.second = false;
    }
}

void RInputEvent::deletePattern(const std::string &pattern)
{
}

void RInputEvent::deletePattern()
{
    keyboardInputs_.clear();
    buttonPattern_.clear();
}
