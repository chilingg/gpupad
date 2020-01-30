#include <RDebug.h>
#include <RThreadPool.h>
#include <RTimer.h>

using namespace Redopera;

bool primeNum(unsigned n)
{
    for(unsigned i = 2; i < n; ++i)
        if(n % i == 0)
            return false;
    return true;
}

std::atomic_int count;

int main()
{
    RTimerNS timer;
    unsigned tNum = std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency() : 1;

    rDebug << "\n================线程测试================\n\n"
              "计算0~50000内的质数数量: \n\n"
              "使用单线程...";

    timer.start();
    count = 0;
    for(unsigned i = 0; i < 50000; ++i)
        if(primeNum(i)) ++count;

    rDebug << "计算结果: " + std::to_string(count)
              + " 用时: " + std::to_string(timer.elapsed()) + "ns\n";

    rDebug << "使用线程池执行单个计算任务(50000)... "
           << "线程数量: " + std::to_string(tNum);

    timer.start();
    count = 0;
    RThreadPool pool; // RThreadPool pool(tNum);
    for(unsigned i = 0; i < 50000; ++i)
    {
        RFunction<bool(unsigned)> f([i]{
            if(primeNum(i)) ++count;
        });
        pool.submit(std::move(f));
    }
    while(!pool.isIdle()) // isIdle()只是查询任务栈是否为空，只有析构才能确保所有任务执行完毕
        std::this_thread::yield();
    pool.waitingForDone();

    rDebug << "计算结果: " + std::to_string(count)
              + " 用时: " + std::to_string(timer.elapsed()) + "ns\n";

    rDebug << "使用多线程分块计算... "
           << "线程数量: " + std::to_string(tNum)
              + " 分块大小: " + std::to_string(50000 / tNum);

    timer.start();
    count = 0;
    std::vector<RThread> threads;
    threads.reserve(tNum);

    unsigned block = 50000 / tNum;
    for(unsigned i = 0; i < tNum; ++i)
    {
        threads.emplace_back([i, block]{
            unsigned begin = i * block;
            unsigned end = begin + block > 50000 ? 50000 : begin + block;
            while(begin < end)
            {
                if(primeNum(begin)) ++count;
                ++begin;
            }
        });
    }
    for(auto &t : threads)
        t.join();

    rDebug << "计算结果: " + std::to_string(count)
              + " 用时: " + std::to_string(timer.elapsed()) + "ns\n";

    return 0;
}
