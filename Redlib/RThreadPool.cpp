#include "RThreadPool.h"

#include "RDebug.h"

RThreadPool* RThreadPool::threadPool = nullptr;

int RThreadPool::initiation(int tNum)
{
    if(!threadPool)
        threadPool = new RThreadPool(tNum);

    return threadPool->threadNumber();
}

RThreadPool *RThreadPool::instance()
{
    return threadPool;
}

int RThreadPool::threadNumber() const
{
    //static int count = std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency() - 1 : 1;
    return threads_.size();
}

RThreadPool::RThreadPool(int tNum):
    done_(false),
    index_(0)
{
    if(tNum < 1)
        tNum = std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency() - 1 : 1;
    assert(tNum > 0);

    stacks_.reserve(tNum);
    threads_.reserve(tNum);
    try {
        for(int i = 0; i < tNum; ++i)
        {
            stacks_.emplace_back(std::make_unique<RThreadStack<FunctionWrapper>>());
            threads_.emplace_back(&RThreadPool::workerThread, this, stacks_[i]);
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

bool RThreadPool::isIdle() const
{
    for(auto &stack : stacks_)
    {
        if(!stack->empty())
            return false;
    }
    return true;
}

void RThreadPool::workerThread(RThreadStack<RThreadPool::FunctionWrapper> *stack)
{
    while(!done_)
    {
        FunctionWrapper task;
        if(stack->tryPop(task))
            task();
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

