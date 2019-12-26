#include "RThreadPool.h"

int RThreadPool::threadNumber()
{
    static int count = std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency() - 1 : 1;
    return count; //线程池至少有一个线程
}

RThreadPool::RThreadPool():
    done_(false),
    index_(0)
{
    unsigned count = threadNumber();
    queues_.reserve(count);
    try {
        for(unsigned i = 0; i < count; ++i)
        {
            queues_.emplace_back(new RThreadStack<FunctionWrapper>);
            threads.push_back(RThread(&RThreadPool::workerThread, this, queues_.back().get(), i));
        }
    } catch(...) {
        done_ = true;
        throw;
    }
}

RThreadPool::~RThreadPool()
{
    done_ = true;
}

bool RThreadPool::runOneTask()
{
    FunctionWrapper task;
    for(auto &queue : queues_)
    {
        if(queue->tryPop(task))
        {
            task();
            return true;
        }
    }
    return false; //线程池任务为空
}

bool RThreadPool::isIdle() const
{
    for(auto &queue : queues_)
    {
        if(!queue->empty())
            return false;
    }
    return true;
}

void RThreadPool::workerThread(RThreadStack<RThreadPool::FunctionWrapper> *queue, unsigned index)
{
    while(!done_)
    {
        FunctionWrapper task;
        if(queue->tryPop(task))
        {
            task();
            index_ = index;
        }
        else if(!runOneTask())
            std::this_thread::yield();
    }
}

template<typename FuncType>
std::future<typename std::result_of<FuncType()>::type> RThreadPool::submit(FuncType f)
{
    typedef typename std::result_of<FuncType()>::type resultType;
    std::packaged_task<resultType()> task(std::move(f));
    std::future<resultType> res(task.get_future());

    for(auto &queue : queues_)
    {
        if(queue->empty())
        {
            queue->push(std::move(task));
            return res;
        }
    }
    //若无空闲线程，则交给最近完成任务的线程
    queues_[index_]->push(std::move(task));
    return res;
}

