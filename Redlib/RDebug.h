//RDebug不是线程安全的，但好像问题不大

#ifndef RDEBUG_H
#define RDEBUG_H

#ifndef R_DEBUG
#define NDEBUG
#endif
#include <assert.h>
#include <iostream>
#include <string>

#include "RPoint.h"
#include "RColor.h"

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

    const RDebug& operator<<(const RPoint2 &p) const;
    const RDebug& operator<<(const RPoint3 &p) const;
    const RDebug& operator<<(const RColor &color) const;

private:
};

inline void printError(const std::string &err)
{
    std::cerr << err << '\n';
}

inline bool printError(bool b, const std::string &err)
{
    if(b)
        std::cerr << err << '\n';
    return b;
}

namespace format
{
static const char non[] = "\033[0m"; //关闭所有属性
static const char bold[] = "\033[1m"; //高亮

static const char black[] = "\033[30m"; //黑
static const char red[] = "\033[31m"; //红
static const char green[] = "\033[32m"; //绿
static const char yellow[] = "\033[33m"; //黄
static const char blue[] = "\033[34m"; //蓝色
static const char purple[] = "\033[35m"; //紫色
static const char cyan[] = "\033[36m"; //深绿
static const char white[] = "\033[37m"; //白色

static const char bblack[] = "\033[40m"; //黑色背景 //设置背景色
static const char bred[] = "\033[41m"; //红色背景
static const char bgreen[] = "\033[42m"; //绿色背景
static const char bbrown[] = "\033[43m"; //棕色背景
static const char bblue[] = "\033[44m"; //蓝色背景
static const char bmagente[] = "\033[45m"; //品红背景
static const char bcyan[] = "\033[46m"; //孔雀蓝背景
static const char bwhite[] = "\033[47m"; //白色背景

static const char clear[] = "\033[2J"; //清屏
};//格式ESC

#endif // RDEBUG_H
