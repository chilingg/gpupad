#include "RDebug.h"

RDebug::RDebug()
{
#ifndef R_NO_DEBUG
#endif
}

RDebug::~RDebug()
{
#ifndef R_NO_DEBUG
    std::cout << std::endl;
#endif
}

const RDebug &RDebug::operator<<(int value) const
{
#ifndef R_NO_DEBUG
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(long value) const
{
#ifndef R_NO_DEBUG
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(long long value) const
{
#ifndef R_NO_DEBUG
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(unsigned value) const
{
#ifndef R_NO_DEBUG
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(unsigned long value) const
{
#ifndef R_NO_DEBUG
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(unsigned long long value) const
{
#ifndef R_NO_DEBUG
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(double value) const
{
#ifndef R_NO_DEBUG
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(float value) const
{
#ifndef R_NO_DEBUG
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(void *p) const
{
#ifndef R_NO_DEBUG
    std::cout << p << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(bool b) const
{
#ifndef R_NO_DEBUG
    const char *str = b ? "true " : "fales ";
    std::cout << str;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(char c) const
{
#ifndef R_NO_DEBUG
    std::cout << c;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(unsigned char c) const
{
#ifndef R_NO_DEBUG
    std::cout << c;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const char *str) const
{
#ifndef R_NO_DEBUG
    std::cout << str;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const unsigned char *c) const
{
#ifndef R_NO_DEBUG
    std::cout << c;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const std::string &str) const
{
#ifndef R_NO_DEBUG
    std::cout << str;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(wchar_t c) const
{
#ifndef R_NO_DEBUG
    std::cout << c;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const wchar_t *str) const
{
#ifndef R_NO_DEBUG
    std::cout << str;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const std::wstring &str) const
{
#ifndef R_NO_DEBUG
    std::wcout << str;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const RPoint &p) const
{
#ifndef R_NO_DEBUG
    std::wcout << '(' << p.x() << ", " << p.y() << ')';
#endif
    return *this;
}
