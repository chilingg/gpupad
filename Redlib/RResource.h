#ifndef RRESOURCE_H
#define RRESOURCE_H

#include <string>

class RResource
{
public:
    static std::string openTextFile(const char *path);
    static std::string checkResourcePath(const std::string &path);

    RResource();
    virtual ~RResource();

    virtual bool isValid() const = 0;

protected:
};

#endif // RRESOURCE_H
