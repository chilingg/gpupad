#include "rdebug.h"

RDebug::RDebug()
{
    if(++count != 1)
    {
        printf("%c", '\n');
        fflush(stdout);
        --count;
    }
}

RDebug::~RDebug()
{
    if(count == 1)
    {
        printf("%c", '\n');
        fflush(stdout);
        --count;
    }
}

const RDebug &RDebug::operator<<(int value) const
{
    printf("%d ", value);
    fflush(stdout);
    return *this;
}

const RDebug &RDebug::operator<<(unsigned value) const
{
    printf("%u ", value);
    fflush(stdout);
    return *this;
}

const RDebug &RDebug::operator<<(double value) const
{
    printf("%f ", value);
    fflush(stdout);
    return *this;
}

const RDebug &RDebug::operator<<(const char *str) const
{
    printf("%s ", str);
    fflush(stdout);
    return *this;
}

const RDebug &RDebug::operator<<(char s) const
{
    printf("%c ", s);
    fflush(stdout);
    return *this;
}

const RDebug &RDebug::operator<<(void *p) const
{
    printf("%p ", p);
    fflush(stdout);
    return *this;
}

const RDebug &RDebug::operator<<(bool b) const
{
    const char *str = b ? "true" : "fales";
    printf("%s ", str);
    fflush(stdout);
    return *this;
}

int RDebug::count = 0;
