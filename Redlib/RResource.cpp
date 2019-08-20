#include "RResource.h"
#include "RDebug.h"
#include <fstream>
#include <sstream>

RResource::RResource():
    state(false)
{

}

RResource::~RResource()
{

}

void RResource::deleteResource()
{

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
