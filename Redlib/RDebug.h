#ifndef RDEBUG_H
#define RDEBUG_H

#include <cstdio>
#include <string>
#include <glm/vec2.hpp>

class RDebug
{
public:
    RDebug();
    ~RDebug();
    RDebug(const RDebug&) = delete;
    RDebug& operator=(const RDebug&) = delete;

    const RDebug& operator<<(int value) const;
    const RDebug& operator<<(long value) const;
    const RDebug& operator<<(long long value) const;
    const RDebug& operator<<(unsigned value) const;
    const RDebug& operator<<(unsigned long value) const;
    const RDebug& operator<<(unsigned long long value) const;
    const RDebug& operator<<(double value) const;
    const RDebug& operator<<(float value) const;
    const RDebug& operator<<(const char *str) const;
    const RDebug& operator<<(const unsigned char *str) const;
    const RDebug& operator<<(char s) const;
    const RDebug& operator<<(std::string s) const;
    const RDebug& operator<<(void* p) const;
    const RDebug& operator<<(bool b) const;
    const RDebug& operator<<(const glm::vec2 &v) const;

private:
    static int count;
};

inline void printErro(const char *error)
{
#ifndef RO_NO_DEBUGE
    fprintf(stderr, "%s\n", error);
    fflush(stderr);
#endif
}

inline void printErro(const std::string &str)
{
#ifndef RO_NO_DEBUGE
    printErro(str.c_str());
#endif
}

#endif // RDEBUG_H
