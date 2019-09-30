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

const RDebug &RDebug::operator<<(float value) const
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

const RDebug &RDebug::operator<<(wchar_t s) const
{
#ifndef RO_NO_DEBUGE
    setlocale(LC_CTYPE, "");
    printf("%C", s);
    setlocale(LC_CTYPE, "C");
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const wchar_t *str) const
{
#ifndef RO_NO_DEBUGE
    setlocale(LC_CTYPE, "");
    printf("%S", str);
    setlocale(LC_CTYPE, "C");
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const std::string &s) const
{
#ifndef RO_NO_DEBUGE
    printf("%s", s.c_str());
    fflush(stdout);
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const std::wstring &s) const
{
#ifndef RO_NO_DEBUGE
    setlocale(LC_CTYPE, "");
    printf("%S", s.c_str());
    setlocale(LC_CTYPE, "C");
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

const RDebug &RDebug::operator<<(const glm::vec2 &v) const
{
#ifndef RO_NO_DEBUGE
    printf("(%f, %f) ", v.x, v.y);
    fflush(stdout);
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const glm::vec4 &v) const
{
#ifndef RO_NO_DEBUGE
    printf("(%f, %f, %f, %f) ", v.x, v.y, v.z, v.w);
    fflush(stdout);
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const glm::mat4 &mat) const
{
#ifndef RO_NO_DEBUGE
    for(int i = 0; i < 4; ++i)
    {
        printf("(%f, %f, %f, %f) \n", mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
    }
    fflush(stdout);
#endif
    return *this;
}

int RDebug::count = 0;
