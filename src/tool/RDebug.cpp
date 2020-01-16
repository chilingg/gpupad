#include "RDebug.h"

using namespace Redopera;

std::atomic_bool RDebug::flag_(false);

RDebug::RDebug()
{
    while(flag_)
        ;
    flag_ = true;
}

RDebug::~RDebug()
{
    std::cout << std::endl;
    flag_ = false;
}

const RDebug &RDebug::operator<<(int value) const
{
    std::cout << value << ' ';
    return *this;
}

const RDebug &RDebug::operator<<(long value) const
{
    std::cout << value << ' ';
    return *this;
}

const RDebug &RDebug::operator<<(long long value) const
{
    std::cout << value << ' ';
    return *this;
}

const RDebug &RDebug::operator<<(unsigned value) const
{
    std::cout << value << ' ';
    return *this;
}

const RDebug &RDebug::operator<<(unsigned long value) const
{
    std::cout << value << ' ';
    return *this;
}

const RDebug &RDebug::operator<<(unsigned long long value) const
{
    std::cout << value << ' ';
    return *this;
}

const RDebug &RDebug::operator<<(double value) const
{
    std::cout << value << ' ';
    return *this;
}

const RDebug &RDebug::operator<<(float value) const
{
    std::cout << value << ' ';
    return *this;
}

const RDebug &RDebug::operator<<(void *p) const
{
    std::cout << p << ' ';
    return *this;
}

const RDebug &RDebug::operator<<(bool b) const
{
    const char *str = b ? "true " : "fales ";
    std::cout << str;
    return *this;
}

const RDebug &RDebug::operator<<(char c) const
{
    std::cout << c;
    return *this;
}

const RDebug &RDebug::operator<<(unsigned char c) const
{
    std::cout << c;
    return *this;
}

const RDebug &RDebug::operator<<(const char *str) const
{
    std::cout << str;
    return *this;
}

const RDebug &RDebug::operator<<(const unsigned char *c) const
{
    std::cout << c;
    return *this;
}

const RDebug &RDebug::operator<<(const std::string &str) const
{
    std::cout << str;
    return *this;
}

const RDebug &RDebug::operator<<(wchar_t c) const
{
#ifdef linux
    bool b = std::ios::sync_with_stdio(false);    // Linux gcc.
#endif

    std::locale loc = std::locale::global(std::locale(""));
    //setlocale(LC_CTYPE, "");    // MinGW gcc.
    std::locale locw = std::wcout.imbue(std::locale(std::locale(), "", LC_CTYPE));

    std::cout << std::flush;
    std::wcout << c << std::ends;

    std::wcout.imbue(locw);
    std::locale::global(loc);

#ifdef linux
    std::ios::sync_with_stdio(b);
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const wchar_t *str) const
{
#ifdef linux
    bool b = std::ios::sync_with_stdio(false);    // Linux gcc.
#endif

    std::locale loc = std::locale::global(std::locale(""));
    //setlocale(LC_CTYPE, "");    // MinGW gcc.
    std::locale locw = std::wcout.imbue(std::locale(std::locale(), "", LC_CTYPE));

    std::cout << std::flush;
    std::wcout << str << std::ends;

    std::wcout.imbue(locw);
    std::locale::global(loc);

#ifdef linux
    std::ios::sync_with_stdio(b);
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const std::wstring &str) const
{
#ifdef linux
    bool b = std::ios::sync_with_stdio(false);    // Linux gcc.
#endif

    std::locale loc = std::locale::global(std::locale(""));
    //setlocale(LC_CTYPE, "");    // MinGW gcc.
    std::locale locw = std::wcout.imbue(std::locale(std::locale(), "", LC_CTYPE));

    std::cout << std::flush;
    std::wcout << str << std::ends;

    std::wcout.imbue(locw);
    std::locale::global(loc);

#ifdef linux
    std::ios::sync_with_stdio(b);
#endif
    return *this;
}

const RDebug &RDebug::operator<<(std::ios_base &(*base)(std::ios_base &)) const
{
    std::cout << base;
    return *this;
}

const RDebug &RDebug::operator<<(std::thread::id id) const
{
    std::cout << id << ' ';
    return *this;
}

const RDebug &RDebug::operator<<(const RColor &color) const
{
    std::cout << "RGBA:(" << color.r()*1u << ", " << color.g()*1u << ", " << color.b()*1u << ", " << color.a()*1u << ") ";
    return *this;
}

/*
const RDebug &RDebug::operator<<(const RPoint2 &p) const
{
    std::cout << '(' << p.x() << ", " << p.y() << ") ";
    return *this;
}

const RDebug &RDebug::operator<<(const RPoint3 &p) const
{
    std::cout << '(' << p.x() << ", " << p.y() << ", " << p.z() << ") ";
    return *this;
}

const RDebug &RDebug::operator<<(const RSize &size) const
{
    std::cout << "(W: " << size.width() << " H: " << size.height() << ") ";
    return *this;
}

const RDebug &RDebug::operator<<(const RRect &rect) const
{
    std::cout << "(" << rect.left() << ", " << rect.bottom() << " | W: " << rect.width() << " H: " << rect.height() << ") ";
    return *this;
}

const RDebug &RDebug::operator<<(const RVector3 &vec) const
{
    std::cout << '(' << vec.x << ", " << vec.y << ", " << vec.z << ") ";
    return *this;
}

const RDebug &RDebug::operator<<(const RVector4 &vec) const
{
    std::cout << '(' << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ") ";
    return *this;
}

const RDebug &RDebug::operator<<(const RMatrix4 &mat) const
{
    //std::cout << std::setw(6);
    std::cout << "mat:(" << std::setw(6) << mat[0][0] << ',' << std::setw(6) <<  mat[1][0] << ','
                         << std::setw(6) << mat[2][0] << ',' << std::setw(6) <<  mat[3][0] << ")\n";
    std::cout << "    (" << std::setw(6) << mat[0][1] << ',' << std::setw(6) <<  mat[1][1] << ','
                         << std::setw(6) << mat[2][1] << ',' << std::setw(6) <<  mat[3][1] << ")\n";
    std::cout << "    (" << std::setw(6) << mat[0][2] << ',' << std::setw(6) <<  mat[1][2] << ','
                         << std::setw(6) << mat[2][2] << ',' << std::setw(6) <<  mat[3][2] << ")\n";
    std::cout << "    (" << std::setw(6) << mat[0][3] << ',' << std::setw(6) <<  mat[1][3] << ','
                         << std::setw(6) << mat[2][3] << ',' << std::setw(6) <<  mat[3][3] << ").";
    return *this;
}

const RDebug &RDebug::operator<<(const RJosn &json) const
{
    std::cout << json;
    return *this;
}

const RDebug &RDebug::operator<<(const RTime &time) const
{
    std::cout << time.toString() << ' ';
    return *this;
}*/
