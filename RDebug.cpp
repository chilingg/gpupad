#include "RDebug.h"

RDebug::RDebug()
{
#ifndef RO_NO_DEBUGE
    if(++count != 1)
    {
        printf("%c", '\n');
        fflush(stdout);
        --count;
    }
#endif
}

RDebug::~RDebug()
{
#ifndef RO_NO_DEBUGE
    if(count == 1)
    {
        printf("%c", '\n');
        fflush(stdout);
        --count;
    }
#endif
}

const RDebug &RDebug::operator<<(int value) const
{
#ifndef RO_NO_DEBUGE
    printf("%d ", value);
    fflush(stdout);
#endif
    return *this;
}

const RDebug &RDebug::operator<<(long value) const
{
#ifndef RO_NO_DEBUGE
    printf("%ld ", value);
    fflush(stdout);
#endif
    return *this;
}

const RDebug &RDebug::operator<<(long long value) const
{
#ifndef RO_NO_DEBUGE
    printf("%lld ", value);
    fflush(stdout);
#endif
    return *this;
}

const RDebug &RDebug::operator<<(unsigned value) const
{
#ifndef RO_NO_DEBUGE
    printf("%u ", value);
    fflush(stdout);
#endif
    return *this;
}

const RDebug &RDebug::operator<<(unsigned long value) const
{
#ifndef RO_NO_DEBUGE
    printf("%lu ", value);
    fflush(stdout);
#endif
    return *this;
}

const RDebug &RDebug::operator<<(unsigned long long value) const
{
#ifndef RO_NO_DEBUGE
    printf("%llu ", value);
    fflush(stdout);
#endif
    return *this;
}

const RDebug &RDebug::operator<<(double value) const
{
#ifndef RO_NO_DEBUGE
    printf("%f ", value);
    fflush(stdout);
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const char *str) const
{
#ifndef RO_NO_DEBUGE
    printf("%s", str);
    fflush(stdout);
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const unsigned char *str) const
{
#ifndef RO_NO_DEBUGE
    printf("%s", str);
    fflush(stdout);
#endif
    return *this;
}

const RDebug &RDebug::operator<<(char s) const
{
#ifndef RO_NO_DEBUGE
    printf("%c", s);
    fflush(stdout);
#endif
    return *this;
}

const RDebug &RDebug::operator<<(std::string s) const
{
#ifndef RO_NO_DEBUGE
    printf("%s", s.c_str());
    fflush(stdout);
#endif
    return *this;
}

const RDebug &RDebug::operator<<(void *p) const
{
#ifndef RO_NO_DEBUGE
    printf("%p ", p);
    fflush(stdout);
#endif
    return *this;
}

const RDebug &RDebug::operator<<(bool b) const
{
#ifndef RO_NO_DEBUGE
    const char *str = b ? "true" : "fales";
    printf("%s ", str);
    fflush(stdout);
#endif
    return *this;
}

int RDebug::count = 0;
