#ifndef RRESOURCE_H
#define RRESOURCE_H

#include <string>

class RResource
{
public:
    RResource();

    bool isValid() const;
    static std::string openTextFile(const char *path);

protected:
    bool valid;
};

inline bool RResource::isValid() const
{
    return valid;
}

#endif // RRESOURCE_H
