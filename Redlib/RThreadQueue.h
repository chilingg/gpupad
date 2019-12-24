#ifndef RTHREADQUEUE_H
#define RTHREADQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class RThreadQueue
{
public:
    RThreadQueue() {}
    RThreadQueue(const RThreadQueue& queue) {
        std::lock_guard lock(mut_);
        data_ = queue.data_;
    }
    RThreadQueue& operator=(const RThreadQueue&) = delete;

    void push(T value) {
        std::lock_guard lock(mut_);
        data_.push(std::move(value));
        cond_.notify_one();
    }

    bool tryPop(T& value) {
        std::lock_guard lock(mut_);
        if(data_.empty()) return false;
        value = std::move(data_.front());
        data_.pop();
        return true;
    }

    std::shared_ptr<T> tryPop() {
        std::lock_guard lock(mut_);
        if(data_.empty()) return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data_.front()));
        data_.pop();
        return res;
    }

    void waitAndPop(T& value) {
        std::unique_lock lock(mut_);
        cond_.wait(lock, [this]{ return !data_.empty(); });
        value = std::move(data_.front());
        data_.pop();
    }
    T waitAndPop() {
        std::unique_lock lock(mut_);
        cond_.wait(lock, [this]{ return !data_.empty(); });
        T res(data_.front());
        data_.pop();
        return res;
    }

    bool empty() const {
        std::lock_guard lock(mut_);
        return data_.empty();
    }

private:
    mutable std::mutex mut_;
    std::queue<T> data_;
    std::condition_variable cond_;
};

#endif // RTHREADQUEUE_H
