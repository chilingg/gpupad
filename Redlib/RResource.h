//1.指向核心数据的智能指针（初始化并作为状态查询标记）
//2.赋值运算符与析构函数
//3.更改时断开共享
//4.实现一个数据有效性查询的纯虚函数

#ifndef RRESOURCE_H
#define RRESOURCE_H

#include <string>

class RResource
{
public:
    static std::string openTextFile(const char *path);
    static std::string checkResourcePath(const std::string &path);

    RResource();
    virtual ~RResource();

    virtual bool isValid() const = 0;

protected:
};

#endif // RRESOURCE_H
