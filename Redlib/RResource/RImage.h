#ifndef RIMAGE_H
#define RIMAGE_H

#include "RResource.h"
#include "Extern/stb_image.h"
#include "RColor.h"

#include <memory>

class RImage : public RResource
{
    friend void swap(RImage &img1, RImage &img2);
public:
    static RImage getRedoperaIcon();

    RImage();
    RImage(const std::string &path, const std::string &name, bool flip = false);
    RImage(int width, int height, int channel, unsigned char *data, const std::string &name);
    RImage(const RImage &img);
    RImage(const RImage &&img);
    RImage& operator=(RImage img);
    ~RImage();
    void swap(RImage &img);

    bool isValid() const;
    int width() const;
    int height() const;
    int channel() const;
    unsigned char* data();
    const unsigned char* cdata() const;

    bool load(std::string path, bool flip = false);
    bool load(const unsigned char *buf, size_t size, bool flip = false);
    bool load(int width, int height, int channel, unsigned char *data);
    void flipVertical();
    void freeImage();
    void full(const RColor &color);

protected:
    void copyOnWrite();

private:
    static const unsigned char RIcon[];

    std::shared_ptr<unsigned char> data_;
    int width_ = 0;
    int height_ = 0;
    int channel_ = 0;
};

void swap(RImage &img1, RImage &img2);

#endif // RIMAGE_H
