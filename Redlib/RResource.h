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
    static constexpr unsigned INVALID = unsigned(-1);
    bool state;
    int *share_ = nullptr;
};

inline bool RResource::isValid() const
{
    return state;
}

#endif // RRESOURCE_H
