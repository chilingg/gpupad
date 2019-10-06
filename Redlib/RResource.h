#ifndef RRESOURCE_H
#define RRESOURCE_H

#include <string>

class RResource
{
public:
    static std::string openTextFile(const char *path);

    RResource();
    RResource(const RResource& rsc);
    RResource& operator=(const RResource& rsc);
    virtual ~RResource();

    virtual void deleteResource() = 0;

    bool isValid() const;

protected:
    static constexpr unsigned INVALID = unsigned(-1);

    void createResource();//待定
    void shaerResource(RResource &target);//待定

    bool state;
    int *share_;
};

inline bool RResource::isValid() const
{
    return state;
}

inline void RResource::createResource()
{
    if(!share_)
    {
        share_ = new int(0);
    }
    else if(*share_ > 0)
    {
        --*share_;
        share_ = new int(0);
    }
}

inline void RResource::shaerResource(RResource &target)
{
    share_ = target.share_;
    ++*share_;
}

#endif // RRESOURCE_H
