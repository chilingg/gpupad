#ifndef RDEBUG_H
#define RDEBUG_H

#include <cstdio>
#include <string>

class RDebug
{
public:
    RDebug();
    ~RDebug();
    RDebug(const RDebug&) = delete;
    RDebug& operator=(const RDebug&) = delete;

    const RDebug& operator<<(int value) const;
    const RDebug& operator<<(unsigned value) const;
    const RDebug& operator<<(double value) const;
    const RDebug& operator<<(const char *str) const;
    const RDebug& operator<<(char s) const;
    const RDebug& operator<<(void* p) const;
    const RDebug& operator<<(bool b) const;

private:
    static int count;
};

inline void printErro(const char *error)
{
    fprintf(stderr, "%s\n", error);
}

inline void printErro(const std::string &str)
{
    printErro(str.c_str());
}

#endif // RDEBUG_H
