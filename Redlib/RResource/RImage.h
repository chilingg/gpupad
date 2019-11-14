#ifndef RIMAGE_H
#define RIMAGE_H

#include "RResource.h"
#include "Extern/stb_image.h"

#include <memory>

class RImage : public RResource
{
public:
    static RImage getRedoperaIcon();

    RImage(const std::string &path = "", bool flip = false);
    ~RImage() override;

    bool load(const std::string &path, bool flip = false);
    int width() const;
    int height() const;
    int channel() const;
    unsigned char* data();
    const unsigned char* cdata() const;

protected:
    void copyOnWrite() override;

private:
    static const unsigned char RIcon[];

    std::shared_ptr<unsigned char> data_;
    int width_ = 0;
    int height_ = 0;
    int channel_ = 0;
};

#endif // RIMAGE_H
