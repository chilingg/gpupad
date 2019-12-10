#include "RDebug.h"

RDebug::RDebug()
{
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
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const wchar_t *str) const
{
#ifdef R_DEBUG
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
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const std::wstring &str) const
{
#ifdef R_DEBUG
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
#endif
    return *this;
}

const RDebug &RDebug::operator<<(std::ios_base &(*base)(std::ios_base &)) const
{
#ifdef R_DEBUG
    std::cout << base;
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const RPoint2 &p) const
{
#ifdef R_DEBUG
    std::cout << '(' << p.x() << ", " << p.y() << ") ";
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const RPoint3 &p) const
{
#ifdef R_DEBUG
    std::cout << '(' << p.x() << ", " << p.y() << ", " << p.z() << ") ";
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const RRect &rect) const
{
#ifdef R_DEBUG
    std::cout << "(" << rect.left() << ", " << rect.bottom() << ")W:" << rect.width() << " H:" << rect.height() << ' ';
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const RColor &color) const
{
#ifdef R_DEBUG
    std::cout << "RGBA:(" << color.r()*1u << ", " << color.g()*1u << ", " << color.b()*1u << ", " << color.a()*1u << ") ";
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const RVector3 &vec) const
{
#ifdef R_DEBUG
    std::cout << '(' << vec.x << ", " << vec.y << ", " << vec.z << ") ";
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const RVector4 &vec) const
{
#ifdef R_DEBUG
    std::cout << '(' << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ") ";
#endif
    return *this;
}

const RDebug &RDebug::operator<<(const RMatrix4 &mat) const
{
#ifdef R_DEBUG
    //std::cout << std::setw(6);
    std::cout << "mat:(" << std::setw(6) << mat[0][0] << ',' << std::setw(6) <<  mat[1][0] << ','
                         << std::setw(6) << mat[2][0] << ',' << std::setw(6) <<  mat[3][0] << ")\n";
    std::cout << "    (" << std::setw(6) << mat[0][1] << ',' << std::setw(6) <<  mat[1][1] << ','
                         << std::setw(6) << mat[2][1] << ',' << std::setw(6) <<  mat[3][1] << ")\n";
    std::cout << "    (" << std::setw(6) << mat[0][2] << ',' << std::setw(6) <<  mat[1][2] << ','
                         << std::setw(6) << mat[2][2] << ',' << std::setw(6) <<  mat[3][2] << ")\n";
    std::cout << "    (" << std::setw(6) << mat[0][3] << ',' << std::setw(6) <<  mat[1][3] << ','
                         << std::setw(6) << mat[2][3] << ',' << std::setw(6) <<  mat[3][3] << ").";
#endif
    return *this;
}
