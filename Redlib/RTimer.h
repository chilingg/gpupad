#ifndef RTIMER_H
#define RTIMER_H

#ifdef win32
#include <windows.h>
#elif linux
#include <unistd.h>
#endif

#include "ROpenGL.h"

class RTimer
{
public:
    RTimer(): startTime(glfwGetTime()) {}
    double elapsed() { return glfwGetTime() - startTime; }
    double elapsed(double time){
        double elp = glfwGetTime() - startTime;
        if(elp <= time) rsleep(time - elp);
        return glfwGetTime() - startTime;
    }
    void rsleep(double seconds) {
#ifdef win32
    unsigned ms = seconds * 1000;
    sleep(ms);
#elif linux
    unsigned us = static_cast<unsigned>(seconds * 1000000); usleep(us);
#endif
    }
    void start() { startTime = glfwGetTime(); }

private:
    double startTime = 0.0;
};

#endif // RTIMER_H
