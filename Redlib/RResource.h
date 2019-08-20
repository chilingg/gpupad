#ifndef RRESOURCE_H
#define RRESOURCE_H

#include <string>

class RResource
{
public:
    RResource();
    RResource(const RResource&) = delete;
    RResource& operator=(const RResource&) = delete;
    ~RResource();

    void deleteResource();
    bool isValid() const;
    static std::string openTextFile(const char *path);

protected:
    bool state;
    static constexpr unsigned INVALID = unsigned(-1);
};

inline bool RResource::isValid() const
{
    return state;
}

#endif // RRESOURCE_H
