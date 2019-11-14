#include "RResource.h"
#include <regex>
#include <fstream>

#include "RDebug.h"

std::string RResource::RESOURCE_PATH = "../Resource/";

RResource::RResource()
{

}

RResource::~RResource()
{

}

std::string RResource::checkFilePath(std::string path)
{
    if(path.empty()) return "";

    static std::regex r("(:/|/|(../)+)?([-_a-z0-9]+/)*[-_a-z0-9]+\\.?[-_a-z0-9]+", std::regex::icase|std::regex::optimize);
    if(!std::regex_match(path, r))
    {
        printError( "Incorrect path: " + path);
        return "";
    }

    if(path[0] == ':')
        path = RESOURCE_PATH + path.substr(2);

    return path;
}

std::string RResource::getTextFileContent(const std::string &path)
{
    auto rcPath = checkFilePath(path);
    if(rcPath.empty())
        return "";

    std::string text;
    std::ifstream file;
    //若状态被置为failbit或badbit，则抛出异常
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(rcPath);
        //读取文件缓冲到数据流
        std::stringstream sstream;
        sstream << file.rdbuf();

        file.close();
        text = sstream.str();
    }
    catch(...)
    {
        printError("Text file done not exist: " + rcPath);
        throw ;
    }

    return text;
}

void RResource::setResourcePath(std::string path)
{
    std::regex r("(/|(../)+)?([-_a-z0-9]+/)+", std::regex::icase);
    if(!std::regex_match(path, r))
    {
        printError( "Incorrect resource path: " + path);
        return ;
    }

    RESOURCE_PATH = path;
}

bool RResource::isValid() const
{
    return valid_;
}

bool RResource::setStatus(bool status)
{
    return valid_ = status;
}

void RResource::invalid()
{
    valid_ = false;
}
