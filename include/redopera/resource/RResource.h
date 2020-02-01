#ifndef RRESOURCE_H
#define RRESOURCE_H

#include <string>
#include <map>
#include <memory>
#include <mutex>

namespace Redopera{

using RData = uint8_t;

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

    static std::string getTextFileContent(const std::string &path);
    static std::string checkFilePath(const std::string &path);
    static void setResourcePath(const std::string &path);
    static const std::string& getResourcePath();

    RResource(const std::string &name, const std::string &typeName);
    RResource(const RResource &rc);
    RResource(const RResource &&rc);
    RResource& operator=(RResource rc);
    void swap(RResource &rc) noexcept;
    ~RResource();

    ResourceID resourceID() const;
    const std::string& name() const;
    std::string nameAndID() const;
    void rename(const std::string &name);

private:
    static ResourceID registerResourceID(const std::string &name, const std::string &typeName);
    static std::shared_ptr<ResourcesList>& resourcesList();
    static void unregisterResourceID(unsigned *ID);

    static std::string resourcesPath;
    static std::mutex mutex;

    std::string name_;
    std::shared_ptr<ResourceID> resourceID_;
};

} // Redopera

void swap(Redopera::RResource &rc1, Redopera::RResource &rc2);

#endif // RRESOURCE_H
