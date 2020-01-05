#ifndef RSIGSLOT_H
#define RSIGSLOT_H

#include <memory>
#include <functional>
#include <mutex>

template<typename ... Args>
class RSignal;

namespace RSigSlot {

template<typename Sloter, typename Sloter2, typename ... Args>
void connect(RSignal<Args ...> *signal, Sloter *sloter, void (Sloter2::*slot)(Args ... args))
{
    //继承RSlot的子类需要在声明尾部展开宏 _RSLOT_TAIL_
    if(typeid(sloter).name() != sloter->TYPE_PTR_NAME)
        throw std::logic_error("Derived class of the RSlot needs to expand _RSLOT_TAIL_ in the tail!");

    auto weakptr = sloter->clone();
    auto func = std::function<bool(Args ... args)>([weakptr, sloter, slot](Args ... args){
        if(weakptr.expired())
            return false;
        (sloter->*slot)(std::forward<Args>(args)...);
        return true;
    });

    std::lock_guard guard(signal->mutex);
    auto it = signal->slots_.find(sloter);
    while(it != signal->slots_.end())
    {
        if(typeid(it->second) == typeid(func))
            return;
    }

    signal->slots_.emplace(sloter, func);
}

template<typename Sloter, typename Sloter2, typename ... Args>
void disconnect(RSignal<Args ...> *signal, Sloter *sloter, void (Sloter2::*slot)(Args ... args))
{
    auto weakptr = sloter->clone();
    auto func = std::function<bool(Args ... args)>([weakptr, slot](Args ... ){ return true; });

    std::lock_guard guard(signal->mutex);
    auto it = signal->slots_.find(sloter);
    while(it != signal->slots_.end())
    {
        if(typeid(it->second) == typeid(func))
        {
            signal->slots_.erase(it);
            return;
        }
    }
}

} // namespace RSigSlot

#define _RSLOT_CLONE_ public: std::weak_ptr<RSlotFlag> clone() { if(!_FLAG_) _FLAG_ = std::make_shared<RSlotFlag>(true); return std::weak_ptr<RSlotFlag>(_FLAG_); }
#define _RSLOT_TYPEID_ const char *TYPE_PTR_NAME = typeid(this).name();
#define _RSLOT_TAIL_ _RSLOT_CLONE_ _RSLOT_TYPEID_ private: std::shared_ptr<RSlotFlag> _FLAG_;

class RSlot
{
    template<typename Sloter, typename Sloter2, typename ... Args>
    friend void RSigSlot::connect(RSignal<Args ...> *signal, Sloter *sloter, void (Sloter2::*slot)(Args ... args));

public:
    using RSlotFlag = bool;

    RSlot() = default;
    ~RSlot() = default;

    _RSLOT_TAIL_  //继承的子类都需要在声明尾部调用此宏
};

template<typename ... Args>
class RSignal
{
    template<typename Sloter, typename Sloter2, typename ... Args2>
    friend void RSigSlot::connect(RSignal<Args2 ...> *signal, Sloter *sloter, void (Sloter2::*slot)(Args2 ... args));

    template<typename Sloter, typename Sloter2, typename ... Args2>
    friend void RSigSlot::disconnect(RSignal<Args2 ...> *signal, Sloter *sloter, void (Sloter2::*slot)(Args2 ... args));

public:
    RSignal() = default;

    void emit(Args ... args)
    {
        std::lock_guard guard(mutex);
        for(auto it = slots_.begin(); it != slots_.end(); ++it)
        {
            bool b = it->second(std::forward<Args>(args)...);
            if(!b)
            {
                it = slots_.erase(it);
                if(it == slots_.end()) break; //??不加不行
            }
        }
    }

private:
    std::mutex mutex;
    std::unordered_multimap<void*, std::function<bool(Args ... args)>> slots_;
};

#endif // RSIGSLOT_H
