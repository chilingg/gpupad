#ifndef RTIMER_H
#define RTIMER_H

#include <thread>
#include <chrono>

class RTimer
{
    using milliseconds =  std::chrono::milliseconds;//毫秒
    using clock =  std::chrono::high_resolution_clock;

public:
    RTimer(): startTime(std::chrono::duration_cast<milliseconds>(clock::now().time_since_epoch()).count()) {}

    long long elapsed() { return std::chrono::duration_cast<milliseconds>(clock::now().time_since_epoch()).count() - startTime; }

    long long elapsed(long long time){
        long long elp = std::chrono::duration_cast<milliseconds>(clock::now().time_since_epoch()).count() - startTime;
        if(elp <= time) std::this_thread::sleep_for(milliseconds(time - elp));
        return std::chrono::duration_cast<milliseconds>(clock::now().time_since_epoch()).count() - startTime;
    }

    void start() { startTime = std::chrono::duration_cast<milliseconds>(clock::now().time_since_epoch()).count(); }

private:
    long long startTime;
};

#endif // RTIMER_H
