#ifndef RTIMER_H
#define RTIMER_H

#include <GLFW/glfw3.h>

#ifdef win32
#include <windows.h>
#elif linux
#include <unistd.h>
#endif

class RTimer
{
public:
    double elapsed();
    double elapsed(double time);
    void rsleep(double seconds);
    void start();

private:
    double startTime = 0.0;
};

#endif // RTIMER_H

inline double RTimer::elapsed()
{
    return glfwGetTime() - startTime;
}

inline double RTimer::elapsed(double time)
{
    double elp = glfwGetTime() - startTime;

    if(elp <= time)
        rsleep(time - elp);

    return glfwGetTime() - startTime;
}

inline void RTimer::rsleep(double seconds)
{
#ifdef win32
    unsigned ms = seconds * 1000;
    sleep(ms);
#elif linux
    unsigned us = seconds * 1000000;
    usleep(us);
#endif
}

inline void RTimer::start()
{
    startTime = glfwGetTime();
}
