//RDebug不是线程安全的，但好像问题不大

#ifndef RDEBUG_H
#define RDEBUG_H

#ifdef R_NO_DEBUG
#define NDEBUG
#endif
#include <assert.h>

#include <iostream>
#include <string>
#include "RPoint.h"

class RDebug
{
public:
    RDebug();
    RDebug(const RDebug&) = delete;
    ~RDebug();
    RDebug& operator=(const RDebug&) = delete;

    const RDebug& operator<<(int value) const;
    const RDebug& operator<<(long value) const;
    const RDebug& operator<<(long long value) const;
    const RDebug& operator<<(unsigned value) const;
    const RDebug& operator<<(unsigned long value) const;
    const RDebug& operator<<(unsigned long long value) const;
    const RDebug& operator<<(double value) const;
    const RDebug& operator<<(float value) const;
    const RDebug& operator<<(void *p) const;
    const RDebug& operator<<(bool b) const;

    const RDebug& operator<<(char c) const;
    const RDebug& operator<<(unsigned char c) const;
    const RDebug& operator<<(const char *str) const;
    const RDebug& operator<<(const unsigned char *c) const;
    const RDebug& operator<<(const std::string &str) const;
    const RDebug& operator<<(wchar_t c) const;
    const RDebug& operator<<(const wchar_t *str) const;
    const RDebug& operator<<(const std::wstring &str) const;

    const RDebug& operator<<(std::ios_base &(*base)(std::ios_base &)) const;

    const RDebug& operator<<(const RPoint &p) const;

private:
};

inline void printError(const std::string &err)
{
    std::cerr << err << "\n";
}

inline bool printError(bool b, const std::string &err)
{
    if(b)
        std::cerr << err << "\n";
    return b;
}

#endif // RDEBUG_H
