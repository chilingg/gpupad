#ifndef RIMAGE_H
#define RIMAGE_H

#include "RResource.h"
#include "stb_image.h"

class RImage : public RResource
{
public:
    RImage();
    RImage(const char* path, bool flip = false);
    RImage(const std::string &path, bool flip = false);
    ~RImage();

    void deleteResource();
    bool load(const char *, bool flip = false);
    int getWidth() const;
    int getHeight() const;
    int channelSize() const;
    const unsigned char *data() const;

private:
    unsigned char *internalData;
    int width;
    int height;
    int channel;
};

inline void RImage::deleteResource()
{
    if(state)
    {
        stbi_image_free(internalData);
        state = false;
    }
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
    return internalData;
}

#endif // RIMAGE_H
