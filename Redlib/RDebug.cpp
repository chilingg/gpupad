#include "RDebug.h"

RDebug::RDebug()
{
#ifndef RO_NO_DEBUGE
    if(++count != 1)
    {
        std::cout << std::endl;
        --count;
    }
#endif
}

RDebug::~RDebug()
{
#ifndef RO_NO_DEBUGE
    if(count == 1)
    {
        std::cout << std::endl;
        --count;
    }
#endif
}

const RDebug &RDebug::operator<<(int value) const
{
#ifndef RO_NO_DEBUGE
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(long value) const
{
#ifndef RO_NO_DEBUGE
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(long long value) const
{
#ifndef RO_NO_DEBUGE
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(unsigned value) const
{
#ifndef RO_NO_DEBUGE
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(unsigned long value) const
{
#ifndef RO_NO_DEBUGE
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(unsigned long long value) const
{
#ifndef RO_NO_DEBUGE
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(double value) const
{
#ifndef RO_NO_DEBUGE
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(float value) const
{
#ifndef RO_NO_DEBUGE
    std::cout << value << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const char *str) const
{
#ifndef RO_NO_DEBUGE
    std::cout << str;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const unsigned char *str) const
{
#ifndef RO_NO_DEBUGE
    std::cout << str;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(char s) const
{
#ifndef RO_NO_DEBUGE
    std::cout << s;
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
    std::cout << s;
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
    std::cout << p;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(bool b) const
{
#ifndef RO_NO_DEBUGE
    const char *str = b ? "true" : "fales";
    std::cout << str;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const glm::vec2 &v) const
{
#ifndef RO_NO_DEBUGE
    std::cout << '(' << v.x << ", " << v.y << ')';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const glm::vec4 &v) const
{
#ifndef RO_NO_DEBUGE
    std::cout << '(' << v.x << ", "
              << v.y << ", "
              << v.z << ", "
              << v.w << ')';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const glm::mat4 &mat) const
{
#ifndef RO_NO_DEBUGE
    std::cout << "mat:(" << mat[0][0] << ' ' <<  mat[1][0] << ' ' <<  mat[2][0] << ' ' <<  mat[3][0] << ")\n";
    std::cout << "    (" << mat[0][1] << ' ' <<  mat[1][1] << ' ' <<  mat[2][1] << ' ' <<  mat[3][1] << ")\n";
    std::cout << "    (" << mat[0][2] << ' ' <<  mat[1][2] << ' ' <<  mat[2][2] << ' ' <<  mat[3][2] << ")\n";
    std::cout << "    (" << mat[0][3] << ' ' <<  mat[1][3] << ' ' <<  mat[2][3] << ' ' <<  mat[3][3] << ")";
#endif
    return *this;
}

int RDebug::count = 0;
