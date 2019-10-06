#ifndef RIMAGE_H
#define RIMAGE_H

#include "RResource.h"
#include "stb_image.h"
#include <memory>

class RImage : public RResource
{
public:
    RImage();
    RImage(const char* path, bool flip = false);
    RImage(const std::string &path, bool flip = false);
    RImage& operator=(const RImage &img);
    ~RImage() override;

    bool isValid() const override;

    bool load(const std::string &path, bool flip = false);
    int getWidth() const;
    int getHeight() const;
    int channelSize() const;
    const unsigned char *data() const;

private:
    std::shared_ptr<unsigned char*> internalData;

    int width = 0;
    int height = 0;
    int channel = 0;
};

inline bool RImage::isValid() const
{
    return *internalData;
}

inline int RImage::getWidth() const
{
    return width;
}

inline int RImage::getHeight() const
{
    return height;
}

inline int RImage::channelSize() const
{
    return channel;
}

inline const unsigned char *RImage::data() const
{
    return *internalData;
}

#endif // RIMAGE_H
