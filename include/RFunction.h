#ifndef RFUNCTION_H
#define RFUNCTION_H

#include <memory>

namespace Redopera {

template<typename Func>
class RFunction;

// 包装只可移动的函数对象
template<typename Result, typename ... Args>
class RFunction<Result(Args ...)>
{
    class ImplementBase
    {
    public:
        virtual void call() = 0;
        virtual ~ImplementBase() {}
    };

    template<typename F>
    class ImplementType : public ImplementBase
    {
    public:
        ImplementType(F&& f): f_(std::move(f)) {}
        void call() { f_(); }
    private:
        F f_;
    };

public:
    RFunction() = default;
    RFunction(RFunction&& func): impl_(std::move(func.impl_)) {}
    RFunction& operator=(RFunction&& func) { impl_ = std::move(func.impl_); return *this; }

    template<typename F>
    RFunction(F&& f): impl_(new ImplementType<F>(std::move(f))) {}

    void operator()() { impl_->call(); }

    RFunction(const RFunction&) = delete;
    RFunction(RFunction&) = delete;
    RFunction& operator=(const RFunction&) = delete;

private:
    std::unique_ptr<ImplementBase> impl_;
};

} // Redopera

#endif // RFUNCTION_H
