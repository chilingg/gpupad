#include "RResource.h"
#include "RDebug.h"
#include <fstream>
#include <sstream>

RResource::RResource():
    state(false),
    share_(new int(0))
{

}

RResource::RResource(const RResource &rsc):
    state(rsc.state),
    share_(rsc.share_)
{
    ++*share_;
}

RResource &RResource::operator=(const RResource &rsc)
{
    auto p = share_;

    share_ = rsc.share_;
    ++*share_;
    state = rsc.state;

    if(*p > 0)
    {
        --*p;
    }
    else
    {
        delete p;xxxxxx
    }

    return *this;
}

RResource::~RResource()
{
    if(*share_ > 0)
    {
        --*share_;
    }
    else
    {
        delete share_;
    }
}

std::string RResource::openTextFile(const char *path)
{
    std::string text;
    std::ifstream file;
    //若状态被置为failbit或badbit，则抛出异常
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        //打开文件
        file.open(path);
        //读取文件的缓冲内容到数据流中
        std::stringstream stream;
        stream << file.rdbuf();

        file.close();
        text = stream.str();
    }
    catch(const std::ifstream::failure &)
    {
        std::string err = "Text reading failure: ";
        err += path;
        printErro(err);
        exit(EXIT_FAILURE);
    }

    return text;
}
