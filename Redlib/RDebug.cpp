#include "RDebug.h"

RDebug::RDebug()
{
#ifdef R_DEBUG
#endif
}

RDebug::~RDebug()
{
#ifdef R_DEBUG
    std::cout << std::endl;
#endif
}

const RDebug &RDebug::operator<<(int value) const
{
#ifdef R_DEBUG
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(long value) const
{
#ifdef R_DEBUG
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(long long value) const
{
#ifdef R_DEBUG
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(unsigned value) const
{
#ifdef R_DEBUG
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(unsigned long value) const
{
#ifdef R_DEBUG
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(unsigned long long value) const
{
#ifdef R_DEBUG
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(double value) const
{
#ifdef R_DEBUG
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(float value) const
{
#ifdef R_DEBUG
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(void *p) const
{
#ifdef R_DEBUG
    std::cout << p << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(bool b) const
{
#ifdef R_DEBUG
    const char *str = b ? "true " : "fales ";
    std::cout << str;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(char c) const
{
#ifdef R_DEBUG
    std::cout << c;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(unsigned char c) const
{
#ifdef R_DEBUG
    std::cout << c;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const char *str) const
{
#ifdef R_DEBUG
    std::cout << str;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const unsigned char *c) const
{
#ifdef R_DEBUG
    std::cout << c;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const std::string &str) const
{
#ifdef R_DEBUG
    std::cout << str;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(wchar_t c) const
{
#ifdef R_DEBUG
    std::cout << c;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const wchar_t *str) const
{
#ifdef R_DEBUG
    std::cout << str;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const std::wstring &str) const
{
#ifdef R_DEBUG
    std::wcout << str;
#endif
    return *this;
}
