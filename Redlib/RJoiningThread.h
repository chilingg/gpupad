#ifndef RJOININGTHREAD_H
#define RJOININGTHREAD_H

#include <thread>
#include <utility>

class RJoiningThread
{
public:
    RJoiningThread() noexcept = default;

    template<typename Callable, typename ... Args>
    explicit RJoiningThread(Callable&& func, Args&& ... args):
        t_(std::forward<Callable>(func), std::forward<Args>(args)...) {}

    explicit RJoiningThread(std::thread t) noexcept:
        t_(std::move(t)) {}

    RJoiningThread(RJoiningThread &&other) noexcept:
        t_(std::move(other.t_)) {}

    RJoiningThread& operator=(RJoiningThread &&other) noexcept
    {
        if(joinable()) join();
        t_ = std::move(other.t_);
        return *this;
    }

    RJoiningThread& operator=(std::thread other) noexcept
    {
        if(joinable()) join();
        t_ = std::move(other);
        return *this;
    }

    ~RJoiningThread() noexcept
    {
        if(joinable()) join();
    }

    void swap(RJoiningThread& other) noexcept
    {
        t_.swap(other.t_);
    }

    std::thread::id threadID() const noexcept { return t_.get_id(); }
    bool joinable() const noexcept { return t_.joinable(); }
    void join() { t_.join(); }
    void detach() { t_.detach(); }

private:
    std::thread t_;
};

#endif // RJOININGTHREAD_H
