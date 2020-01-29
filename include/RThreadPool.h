#ifndef RTHEADPOOL_H
#define RTHEADPOOL_H

#include "RThreadStack.h"
#include "RThread.h"
#include "RFunction.h"

#include <atomic>
#include <future>
#include <vector>

namespace Redopera {

class RThreadPool
{
public:
    RThreadPool(int tNum = 0);
    ~RThreadPool();

    bool isIdle() const;
    int threadNumber() const;

    template<typename FuncType>
    std::future<typename std::result_of<FuncType()>::type> submit(FuncType f);

    bool runOneTask(); //主动扒拉一个任务到当前线程处理

private:
    void workerThread(RThreadStack<RFunction<void()>> *stack);

    std::atomic_bool done_;
    std::atomic_uint index_;
    std::vector<std::unique_ptr<RThreadStack<RFunction<void()>>>> stacks_; // 线程任务栈
    std::vector<RThread> threads_;
};

} // Redopera

#endif // RTHEADPOOL_H
