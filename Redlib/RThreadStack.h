#ifndef RTHREADSTACK_H
#define RTHREADSTACK_H

#include <stack>
#include <mutex>
#include <condition_variable>

template <typename T>
class RThreadStack
{
public:
    RThreadStack() {}
    RThreadStack(const RThreadStack& queue) {
        std::lock_guard lock(mut_);
        data_ = queue.data_;
    }
    RThreadStack& operator=(const RThreadStack&) = delete;

    void push(T value) {
        std::lock_guard lock(mut_);
        data_.push(std::move(value));
        cond_.notify_one();
    }

    bool tryPop(T& value) {
        std::lock_guard lock(mut_);
        if(data_.empty()) return false;
        value = std::move(data_.top());
        data_.pop();
        return true;
    }

    std::shared_ptr<T> tryPop() {
        std::lock_guard lock(mut_);
        if(data_.empty()) return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data_.top()));
        data_.pop();
        return res;
    }

    void waitAndPop(T& value) {
        std::unique_lock lock(mut_);
        cond_.wait(lock, [this]{ return !data_.empty(); });
        value = std::move(data_.top());
        data_.pop();
    }
    T waitAndPop() {
        std::unique_lock lock(mut_);
        cond_.wait(lock, [this]{ return !data_.empty(); });
        T res(data_.top());
        data_.pop();
        return res;
    }

    bool empty() const {
        std::lock_guard lock(mut_);
        return data_.empty();
    }

private:
    mutable std::mutex mut_;
    std::stack<T> data_;
    std::condition_variable cond_;
};

#endif // RTHREADSTACK_H
