#ifndef RRESOURCE_H
#define RRESOURCE_H

#include <string>
#include <vector>

class RResource
{
    friend void swap(RResource &rc1, RResource &rc2);
public:
    using ResourceID = std::vector<RResource*>::size_type;
    using ResourcesList = std::vector<RResource*>;

    static std::string getTextFileContent(std::string path);
    static std::string checkFilePath(std::string path);
    static void setResourcePath(const std::string &path);
    static const ResourcesList *queryResourceList();

    RResource(const std::string &name);
    RResource(const RResource &rc);
    RResource(const RResource &&rc);
    RResource& operator=(RResource rc);
    void swap(RResource &rc);
    ~RResource();

    ResourceID resourceID() const;
    const std::string& name() const;
    std::string nameID() const;
    void rename(const std::string &name);

protected:

private:
    static ResourceID creatorResourceID(RResource *rc);
    static ResourcesList& resourcesList();

    static std::string RESOURCE_PATH;

    std::string name_;
    const ResourceID resourceID_;
};

void swap(RResource &rc1, RResource &rc2);

#endif // RRESOURCE_H
