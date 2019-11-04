#ifndef RRESOURCE_H
#define RRESOURCE_H

#include <string>

class RResource
{
public:
    RResource();
    virtual ~RResource();

    static std::string checkFilePath(std::string path);
    static std::string getTextFileContent(const std::string &path);
    static void setResourcePath(std::string path);

    bool isValid() const;
    bool setStatus(bool status = true);
    void invalid();

protected:
    virtual void copyOnWrite() = 0;

private:
    static std::string RESOURCE_PATH;
    bool valid_ = false;
};

#endif // RRESOURCE_H
