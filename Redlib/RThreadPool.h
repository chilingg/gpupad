#ifndef RTHREADPOOL_H
#define RTHREADPOOL_H

#include <thread>
#include <atomic>
#include <future>
#include <vector>

#include "RThreadStack.h"
#include "RThread.h"

class RThreadPool
{
    class FunctionWrapper //包装只可移动的函数对象为成可复制构造的
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
        FunctionWrapper() = default;
        FunctionWrapper(FunctionWrapper&& func): impl_(std::move(func.impl_)) {}
        FunctionWrapper& operator=(FunctionWrapper&& func) { impl_ = std::move(func.impl_); return *this; }

        template<typename F>
        FunctionWrapper(F&& f): impl_(new ImplementType<F>(std::move(f))) {}

        void operator()() { impl_->call(); }

        FunctionWrapper(const FunctionWrapper&) = delete;
        FunctionWrapper(FunctionWrapper&) = delete;
        FunctionWrapper& operator=(const FunctionWrapper&) = delete;

    private:
        std::unique_ptr<ImplementBase> impl_;
    };

public:
    static int threadNumber();

    RThreadPool();
    ~RThreadPool();

    template<typename FuncType>
    std::future<typename std::result_of<FuncType()>::type> submit(FuncType f);

    bool runOneTask(); //主动扒拉一个任务到当前线程处理
    bool isIdle() const;

private:
    void workerThread(RThreadStack<FunctionWrapper> *queue, unsigned index);

    std::atomic_bool done_;
    std::atomic_uint index_; //标记一个最近完成任务的线程
    std::vector<std::unique_ptr<RThreadStack<FunctionWrapper>>> queues_;
    std::vector<RThread> threads;
};

#endif // RTHREADPOOL_H
