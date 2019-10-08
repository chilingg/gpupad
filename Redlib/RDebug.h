#ifndef RDEBUG_H
#define RDEBUG_H

#include <iostream>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <RVolume.h>

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
    const RDebug& operator<<(wchar_t s) const;
    const RDebug& operator<<(const wchar_t *str) const;
    const RDebug& operator<<(const std::string &s) const;
    const RDebug& operator<<(const std::wstring &s) const;
    const RDebug& operator<<(void* p) const;
    const RDebug& operator<<(bool b) const;
    const RDebug& operator<<(const glm::vec2 &v) const;
    const RDebug& operator<<(const glm::vec4 &v) const;
    const RDebug& operator<<(const glm::mat4 &mat) const;
    const RDebug& operator<<(const RVolume &volume) const;

private:
    static int count;
};

inline void printErro(const char *error)
{
#ifndef RO_NO_DEBUGE
    std::cerr << error << std::endl;
#endif
}

inline void printErro(const std::string &str)
{
#ifndef RO_NO_DEBUGE
    printErro(str.c_str());
#endif
}

#endif // RDEBUG_H
