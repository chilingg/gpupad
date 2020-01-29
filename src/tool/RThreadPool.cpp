#include "RThreadPool.h"
#include <assert.h>

using namespace Redopera;

RThreadPool::RThreadPool(int tNum):
    done_(false),
    index_(0)
{
    if(tNum < 1)
        tNum = std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency() : 1;
    assert(tNum > 0);

    stacks_.reserve(tNum);
    threads_.reserve(tNum);
    try {
        for(int i = 0; i < tNum; ++i)
        {
            stacks_.emplace_back(std::make_unique<RThreadStack<RFunction<void()>>>());
            threads_.emplace_back(&RThreadPool::workerThread, this, stacks_[i].get());
        }
    }
    catch(...) {
        done_ = true;
        throw;
    }
}

RThreadPool::~RThreadPool()
{
    done_ = true;
}

bool RThreadPool::isIdle() const
{
    for(auto &stack : stacks_)
    {
        if(!stack->empty())
            return false;
    }
    return true;
}

int RThreadPool::threadNumber() const
{
    return threads_.size();
}

bool RThreadPool::runOneTask()
{
    RFunction<void()> task;
    for(auto &stack : stacks_)
    {
        if(stack->tryPop(task))
        {
            task();
            return true;
        }
    }
    return false; //线程池任务为空
}

void RThreadPool::workerThread(RThreadStack<RFunction<void ()> > *stack)
{
    while(!done_)
    {
        RFunction<void()> task;
        if(stack->tryPop(task))
            task();
        else if(!runOneTask())
        {
            stack->waitAndPop(task);
            task();
        }
    }
}

template<typename FuncType>
std::future<typename std::result_of<FuncType()>::type> RThreadPool::submit(FuncType f)
{
    typedef typename std::result_of<FuncType()>::type resultType;
    std::packaged_task<resultType()> task(std::move(f));
    std::future<resultType> res(task.get_future());

    for(auto &stack : stacks_)
    {
        if(stack->empty())
        {
            stack->push(std::move(task));
            return res;
        }
    }
    //若无空闲线程，则轮次循环递交
    stacks_[++index_ % threads_.size()]->push(std::move(task));
    return res;
}
