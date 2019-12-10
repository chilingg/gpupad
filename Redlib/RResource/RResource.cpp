#include "RResource.h"
#include <regex>
#include <fstream>

#include "RDebug.h"

std::string RResource::RESOURCE_PATH = "../Resource/";

RResource::RResource(const std::string &name):
    name_(name),
    resourceID_(creatorResourceID(this))
{

}

RResource::RResource(const RResource &rc):
    name_(rc.name_),
    resourceID_(creatorResourceID(this))
{

}

RResource::RResource(const RResource &&rc):
    name_(rc.name_),
    resourceID_(creatorResourceID(this))
{

}

RResource &RResource::operator=(RResource rc)
{
    name_.swap(rc.name_);

    return *this;
}

RResource::~RResource()
{
    resourcesList()[resourceID_] = nullptr;
}

void RResource::swap(RResource &rc)
{
    name_.swap(rc.name_);
}

std::string RResource::checkFilePath(std::string path)
{
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

std::string RResource::getTextFileContent(std::string path)
{
    path = checkFilePath(path);
    if(path.empty())
        return "";

    std::string text;
    std::ifstream file;
    //若状态被置为failbit或badbit，则抛出异常
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(path);
        //读取文件缓冲到数据流
        std::stringstream sstream;
        sstream << file.rdbuf();

        file.close();
        text = sstream.str();
    }
    catch(...)
    {
        printError("Text file done not exist: " + path);
        return "";
    }

    return text;
}

void RResource::setResourcePath(const std::string &path)
{
    std::regex r("(/|(../)+)?([-_a-z0-9]+/)+", std::regex::icase);
    if(!std::regex_match(path, r))
    {
        printError( "Incorrect resource path: " + path);
        return ;
    }

    RESOURCE_PATH = path;
}

const RResource::ResourcesList *RResource::queryResourceList()
{
    return &resourcesList();
}

void swap(RResource &rc1, RResource &rc2)
{
    rc1.name_.swap(rc2.name_);
}

RResource::ResourceID RResource::resourceID() const
{
    return resourceID_;
}

const std::string &RResource::name() const
{
    return name_;
}

std::string RResource::nameID() const
{
    return name() + '(' + std::to_string(resourceID_) + ')';
}

void RResource::rename(const std::string &name)
{
#ifdef R_DEBUG
    if(printError(name.empty(), "The resource name set is empty!"))
        return;
#endif
    name_ = name;
}

RResource::ResourceID RResource::creatorResourceID(RResource *rc)
{
    ResourceID i = 1;
    while(i < resourcesList().size())
    {
        if(!resourcesList()[i])
        {
            resourcesList()[i] = rc;
            return i;
        }
        ++i;
    }
    resourcesList().push_back(rc);
    //返回的是递增了的i
    return i;
}

RResource::ResourcesList &RResource::resourcesList()
{
    static ResourcesList rcList(128, nullptr);
    return rcList;
}
