#include "RResource.h"
#include "RDebug.h"

#include <fstream>
#include <sstream>
#include <regex>

using namespace Redopera;

const std::shared_ptr<RResource::ResourcesList> RResource::queryResourceList()
{
    std::lock_guard<std::mutex> guard(mutex);
    return resourcesList();
}

std::string RResource::getTextFileContent(std::string &path)
{
    if(!checkFilePath(path))
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
        printError("Text file read error: " + path);
        return "";
    }

    return text;
}

bool RResource::checkFilePath(std::string &path)
{
    static std::regex r("(:/|/|(../)+)?([-_a-z0-9]+/)*[-_a-z0-9]+\\.?[-_a-z0-9]+", std::regex::icase|std::regex::optimize);
    if(!std::regex_match(path, r))
    {
        printError( "Incorrect path: " + path);
        return false;
    }

    if(path[0] == ':')
        path = resourcesPath + path.substr(2);

    return true;
}

void RResource::setResourcePath(const std::string &path)
{
    static std::regex r("(/|(../)+)?([-_a-z0-9]+/)+", std::regex::icase|std::regex::optimize);
    if(printError(!std::regex_match(path, r), "Invalid resource path: " + path))
        return;
    resourcesPath = path;
}

const std::string &RResource::getResourcePath()
{
    return resourcesPath;
}

RResource::RResource(const std::string &name):
    name_(name),
    resourceID_(std::make_shared<ResourceID>(registerResourceID()))
{

}

RResource::RResource(const RResource &rc):
    name_(rc.name_),
    resourceID_(rc.resourceID_)
{

}

RResource::RResource(const RResource &&rc):
    name_(std::move(rc.name_)),
    resourceID_(std::move(rc.resourceID_))
{

}

RResource &RResource::operator=(RResource rc)
{
    name_ = rc.name_;
    resourceID_ = rc.resourceID_;
    return *this;
}

void RResource::swap(RResource &rc) noexcept
{
    using std::swap;
    name_.swap(rc.name_);
    resourceID_.swap(rc.resourceID_);
}

RResource::~RResource()
{
    if(resourceID_.unique())
    {
        std::lock_guard<std::mutex> guard(mutex);

        assert(resourcesList()->count(*resourceID_));
        if(!resourcesList().unique())
            resourcesList() = std::make_shared<ResourcesList>(*resourcesList());
        resourcesList()->erase(*resourceID_);
    }
}

RResource::ResourceID RResource::resourceID() const
{
    return *resourceID_;
}

const std::string &RResource::name() const
{
    return name_;
}

std::string RResource::nameAndID() const
{
    return '(' + std::to_string(*resourceID_) + ')' + name_;
}

void RResource::rename(const std::string &name)
{
    name_ = name;
}

RResource::ResourceID RResource::registerResourceID(RResource *rc)
{
    ResourceID i = 0;
    while(!resourcesList()->count(i))
        ++i;

    resourcesList()->emplace(i, ResourceInfo{ rc->name_, typeid(rc).name() });
    return i;
}

std::shared_ptr<RResource::ResourcesList> &RResource::resourcesList()
{
    static std::shared_ptr<ResourcesList> RESOURCE_LIST(new ResourcesList);
    return RESOURCE_LIST;
}
