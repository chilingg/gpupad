#ifndef RTIMER_H
#define RTIMER_H

#include <GLFW/glfw3.h>

class RTimer
{
public:
    double elapsed();
    double elapsed(double time);
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
    while(glfwGetTime() - startTime <= time)
        ;

    return glfwGetTime() - startTime;
}

inline void RTimer::start()
{
    startTime = glfwGetTime();
}
