#ifndef RRESOURCE_H
#define RRESOURCE_H

#include <string>
#include <map>
#include <memory>
#include <mutex>

namespace Redopera{

class RResource
{
    struct ResourceInfo
    {
        std::string name;
        std::string typeName;
    };

    friend void swap(RResource &rc1, RResource &rc2);
    using ResourcesList = std::map<unsigned, ResourceInfo>;

public:
    using ResourceID = unsigned;

    static const std::shared_ptr<ResourcesList> queryResourceList();

    static std::string getTextFileContent(std::string &path);
    static bool checkFilePath(std::string &path);
    static void setResourcePath(const std::string &path);
    static const std::string& getResourcePath();

    RResource(const std::string &name);
    RResource(const RResource &rc);
    RResource(const RResource &&rc);
    RResource& operator=(RResource rc);
    void swap(RResource &rc) noexcept;
    virtual ~RResource();

    ResourceID resourceID() const;
    const std::string& name() const;
    std::string nameAndID() const;
    void rename(const std::string &name);

private:
    static ResourceID registerResourceID(RResource *rc);
    static std::shared_ptr<ResourcesList>& resourcesList();

    static std::string resourcesPath;
    static std::mutex mutex;

    std::string name_;
    std::shared_ptr<ResourceID> resourceID_;
};

} // Redopera

void swap(Redopera::RResource &rc1, Redopera::RResource &rc2);

#endif // RRESOURCE_H
