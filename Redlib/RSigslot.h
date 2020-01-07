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
    // 使用槽函数的类需要在声明尾部展开宏_RSLOT_TAIL_
    if(typeid(Sloter*).hash_code() != sloter->__RSLOT__.sloterTypeHash)
        throw std::logic_error(std::string(typeid(Sloter).name())
                               + ": Using slot functions need to expand the macro _RSLOT_TAIL_ at the tail of the declaration");

    auto weakptr = sloter->__RSLOT__.clone();
    auto func = std::function<bool(Args ... args)>([weakptr, sloter, slot](Args ... args){
        auto sp = weakptr.lock();
        //RDebug() << !sp;
        if(!sp) return false;

        (sloter->*slot)(std::forward<Args>(args)...);
        return true;
    });

    std::lock_guard guard(signal->mutex_);
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
    auto weakptr = sloter->__RSLOT__.clone();
    auto func = std::function<bool(Args ... args)>([weakptr, slot](Args ... ){ return true; });

    std::lock_guard guard(signal->mutex_);
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

class RSlot
{
    template<typename Sloter, typename Sloter2, typename ... Args2>
    friend void RSigSlot::connect(RSignal<Args2 ...> *signal, Sloter *sloter, void (Sloter2::*slot)(Args2 ... args));

    template<typename Sloter, typename Sloter2, typename ... Args2>
    friend void disconnect(RSignal<Args2 ...> *signal, Sloter *sloter, void (Sloter2::*slot)(Args2 ... args));

public:
    RSlot(size_t typeHash):
        sloterTypeHash(typeHash),
        slotFlag(std::make_shared<bool>(true))
    {}

    ~RSlot()
    {
        while(!slotFlag.unique())
            ; // 等待所有槽函数执行完毕才会销毁RSlot，若一直有槽函数执行则死循
        slotFlag.reset();
    }

private:
    std::weak_ptr<bool> clone() const { return std::weak_ptr<bool>(slotFlag); }

    const size_t sloterTypeHash; // 拥有RSlot的类的指针的类型哈希值，用于在connect中确定该类有自己的RSlot而不是继承而来的
    std::shared_ptr<bool> slotFlag; // 存活标志
};

} // namespace RSigSlot

// 在需要使用槽函数的类声明尾部展开宏 _RSLOT_TAIL_
#define _RSLOT_TAIL_ public: const RSigSlot::RSlot __RSLOT__ { typeid(this).hash_code() };

template<typename ... Args>
class RSignal
{
    template<typename Sloter, typename Sloter2, typename ... Args2>
    friend void RSigSlot::connect(RSignal<Args2 ...> *signal, Sloter *sloter, void (Sloter2::*slot)(Args2 ... args));

    template<typename Sloter, typename Sloter2, typename ... Args2>
    friend void RSigSlot::disconnect(RSignal<Args2 ...> *signal, Sloter *sloter, void (Sloter2::*slot)(Args2 ... args));

public:
    RSignal() = default;
    RSignal(const RSignal &) = delete;
    RSignal(RSignal &&sig): slots_(std::move(sig.slots_)) {}
    RSignal& operator=(const RSignal &) = delete;
    RSignal& operator=(RSignal &&sig) { slots_.swap(sig.slots_); };

    void emit(Args ... args)
    {
        std::lock_guard lock(mutex_);
        for(auto it = slots_.begin(); it != slots_.end(); ++it)
        {
            bool b = it->second(std::forward<Args>(args)...);
            if(!b)
            {
                it = slots_.erase(it);
                if(it == slots_.end()) break; //初始单个槽函数返回false的情况下，不加不行
            }
        }
    }

private:
    std::mutex mutex_;
    std::unordered_multimap<void*, std::function<bool(Args ... args)>> slots_;
};

#endif // RSIGSLOT_H
