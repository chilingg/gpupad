#ifndef RRESOURCE_H
#define RRESOURCE_H

#include <string>

class RResource
{
public:
    RResource();
    ~RResource();

    static std::string checkFilePath(std::string path);
    static std::string getTextFileContent(std::string path);
    static void setResourcePath(std::string path);

private:
    static std::string RESOURCE_PATH;
};

#endif // RRESOURCE_H
