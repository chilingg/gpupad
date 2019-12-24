#ifndef RTHREADPOOL_H
#define RTHREADPOOL_H

#include <thread>
#include <atomic>
#include <future>
#include "RThreadQueue.h"
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
        template<typename F>
        FunctionWrapper(F&& f): impl_(new ImplementType<F>(std::move(f))) {}
        FunctionWrapper& operator=(FunctionWrapper&& func)
        {
            impl_ = std::move(func.impl_);
            return *this;
        }

        void operator()() { impl_->call(); }

        FunctionWrapper(const FunctionWrapper&) = delete;
        FunctionWrapper(FunctionWrapper&) = delete;
        FunctionWrapper& operator=(const FunctionWrapper&) = delete;

    private:
        std::unique_ptr<ImplementBase> impl_;
    };

public:
    static unsigned getHardwareConcurrency()
    {
        static unsigned count = std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 1;
        return count;
    }

    RThreadPool(): done_(false)
    {
        const unsigned threadCount = getHardwareConcurrency();
        try {
            for(unsigned i = 0; i < threadCount; ++i)
                threads.push_back(RThread(&RThreadPool::workerThread, this));
        } catch(...) {
            done_ = true;
            throw;
        }
    }

    ~RThreadPool() { done_ = true; }

    template<typename FuncType>
    std::future<typename std::result_of<FuncType()>::type> submit(FuncType f)
    {
        typedef typename std::result_of<FuncType()>::type resultType;
        std::packaged_task<resultType()> task(std::move(f));
        std::future<resultType> res(task.get_future());
        queue_.push(std::move(task));
        return res;
    }

    void runTask() //主动扒拉一个任务到一个线程处理
    {
        FunctionWrapper task;
        if(queue_.tryPop(task)) task();
        else std::this_thread::yield();
    }

private:
    void workerThread()
    {
        while(!done_)
        {
            FunctionWrapper task;
            if(queue_.tryPop(task)) task();
            else std::this_thread::yield();
        }
    }

    std::atomic_bool done_;
    RThreadQueue<FunctionWrapper> queue_;
    std::vector<RThread> threads;
};

#endif // RTHREADPOOL_H
