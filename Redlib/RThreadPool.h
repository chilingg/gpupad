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
    static int initiation(int tNum);
    static RThreadPool* instance();

    ~RThreadPool();

    template<typename FuncType>
    std::future<typename std::result_of<FuncType()>::type> submit(FuncType f);

    bool isIdle() const;
    int threadNumber() const;

    bool runOneTask(); //主动扒拉一个任务到当前线程处理

private:
    static RThreadPool* threadPool;

    RThreadPool(int tNum = 0);

    void workerThread(RThreadStack<FunctionWrapper> *stack);

    std::atomic_bool done_;
    std::atomic_uint index_;
    std::vector<std::unique_ptr<RThreadStack<FunctionWrapper>>> stacks_; // 线程任务栈
    std::vector<RThread> threads_;
};

#endif // RTHREADPOOL_H
