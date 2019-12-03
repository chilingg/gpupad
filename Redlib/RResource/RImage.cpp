#include "RImage.h"

#include "RDebug.h"

void swap(RImage &img1, RImage &img2)
{
    img1.swap(img2);
}

RImage RImage::getRedoperaIcon()
{
    RImage icon;
    icon.width_ = 32;
    icon.height_ = 32;
    icon.channel_ = 4;

    size_t size = 4096;
    unsigned char *data = static_cast<unsigned char*>(calloc(size, sizeof(unsigned char)));
    for(size_t i = 0; i < size; ++i)
        data[i] = RIcon[i];
    icon.data_.reset(data, stbi_image_free);

    return icon;
}

RImage::RImage():
    RResource("UnknowImage")
{

}

RImage::RImage(const std::string &path, const std::string &name, bool flip):
    RResource(name)
{
    load(path, flip);
}

RImage::RImage(int width, int height, int channel, unsigned char *data, const std::string &name):
    RResource(name)
{
    load(width, height, channel, data);
}

RImage::RImage(const RImage &img):
    RResource(img),
    data_(img.data_),
    width_(img.width_),
    height_(img.height_),
    channel_(img.channel_)
{

}

RImage::RImage(const RImage &&img):
    RResource(img),
    data_(img.data_),
    width_(img.width_),
    height_(img.height_),
    channel_(img.channel_)
{
}

RImage &RImage::operator=(RImage img)
{
    swap(img);
    return *this;
}

RImage::~RImage()
{
}

void RImage::swap(RImage &img)
{
    RResource::swap(img);
    using std::swap;
    swap(data_, img.data_);
    swap(width_, img.width_);
    swap(height_, img.height_);
    swap(channel_, img.channel_);
}

bool RImage::load(std::string path, bool flip)
{
    path = checkFilePath(path);
    if(path.empty())
        return false;

    stbi_set_flip_vertically_on_load(flip);
    data_.reset(stbi_load(path.c_str(), &width_, &height_, &channel_,0), stbi_image_free);

    if(!data_)
    {
#ifdef R_DEBUG
        printError(nameID() + ": Image failed to load at path: " + path);
#endif
        data_.reset();
        return false;
    }

    return true;
}

bool RImage::load(int width, int height, int channel, unsigned char *data)
{
    size_t size = static_cast<size_t>(height) * static_cast<size_t>(width) * static_cast<size_t>(channel);
    unsigned char *d = static_cast<unsigned char*>(calloc(size, sizeof(unsigned char)));
    assert(d != nullptr);

    data_.reset(d, stbi_image_free);
    width_ = width;
    height_ = height;
    channel_ = channel;

    if(!data) return true;

    for(size_t i = 0; i < size; ++i)
        d[i] = data[i];

    return true;
}

void RImage::flipVertical()
{
    unsigned char *bitmap = data_.get();
    using std::swap;

    for(int h = 0, h2 = height_ - 1; h < height_/2; ++h, --h2)
    {
        for(int w = 0, w2 = width_ - 1; w < width_/2; ++w, --w2)
        {
            for(int c = 0; c < channel_; ++c)
                swap(bitmap[(h*width_+w)*channel_+c], bitmap[(h2*width_+w2)*channel_+c]);
        }
    }
}

void RImage::freeImage()
{
    data_.reset();
}

void RImage::full(RColor color)
{
    unsigned char *data = data_.get();
    for(int i = 0; i < height_; ++i)
    {
        for(int j = 0; j < width_; ++j)
        {
            data[i*width_*channel_ + j*channel_] = color.r();
            if(channel_ >= 2) data[i*width_*channel_ + j*channel_ + 1] = color.g();
            if(channel_ >= 3) data[i*width_*channel_ + j*channel_ + 2] = color.b();
            if(channel_ == 4) data[i*width_*channel_ + j*channel_ + 3] = color.a();
        }
    }
}

bool RImage::isValid() const
{
    return data_ != nullptr;
}

int RImage::width() const
{
    return width_;
}

int RImage::height() const
{
    return height_;
}

int RImage::channel() const
{
    return channel_;
}

const unsigned char *RImage::cdata() const
{
    return data_.get();
}

unsigned char *RImage::data()
{
    copyOnWrite();
    return data_.get();
}

void RImage::copyOnWrite()
{
    if(data_.unique() || data_ == nullptr) return;
    RDebug() << data_.unique() << data_.use_count();
    assert(isValid());

    size_t size = static_cast<size_t>(height_) * static_cast<size_t>(width_) * static_cast<size_t>(channel_);
    unsigned char *source = data_.get();
    unsigned char *data = static_cast<unsigned char*>(calloc(size, sizeof(unsigned char)));
    for(size_t i = 0; i < size; ++i)
        data[i] = source[i];
    data_.reset(data, stbi_image_free);
}

const unsigned char RImage::RIcon[] = {
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0x1c,0x1c,0x9,0x13,0x13,0x1f,0x52,
    0x15,0x15,0x1d,0x93,0x14,0x14,0x1e,0xc1,0x14,0x14,0x1e,0xe5,0x14,0x14,0x1e,0xef,0x14,0x14,0x1e,0xef,0x14,0x14,0x1e,0xe4,
    0x14,0x14,0x1e,0xc1,0x13,0x13,0x1e,0x92,0x13,0x13,0x1f,0x51,0x1c,0x1c,0x1c,0x9,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x18,0x18,0x20,0x20,0x15,0x15,0x1d,0x94,
    0x14,0x14,0x1e,0xf1,0x14,0x14,0x1e,0xff,0x15,0x15,0x1f,0xff,0x48,0x48,0x4f,0xff,0x7b,0x7b,0x81,0xff,0x3e,0x3e,0x47,0xff,
    0x3e,0x3e,0x47,0xff,0x7b,0x7b,0x81,0xff,0x47,0x47,0x4f,0xff,0x15,0x15,0x1f,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xf1,
    0x15,0x15,0x1d,0x94,0x10,0x10,0x21,0x1f,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2b,0x6,0x14,0x14,0x1d,0x82,
    0x14,0x14,0x1e,0xf8,0x14,0x14,0x1e,0xff,0x1a,0x1a,0x24,0xff,0x79,0x79,0x7f,0xff,0xe4,0xe4,0xe5,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0x64,0x64,0x6b,0xff,0x64,0x64,0x6b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe4,0xe4,0xe5,0xff,
    0x78,0x78,0x7e,0xff,0x1a,0x1a,0x24,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xf7,0x14,0x14,0x1e,0x80,0x0,0x0,0x33,0x5,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xd,0xd,0x1b,0x13,
    0x14,0x14,0x1e,0xbe,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x3f,0x3f,0x47,0xff,0xdd,0xdd,0xde,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x64,0x64,0x6b,0xff,0x64,0x64,0x6b,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xdb,0xdb,0xdd,0xff,0x3e,0x3e,0x47,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xc3,0x17,0x17,0x23,0x16,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x13,0x13,0x20,0x28,0x14,0x14,0x1e,0xde,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x5a,0x5a,0x61,0xff,0xf7,0xf7,0xf7,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x64,0x64,0x6b,0xff,
    0x64,0x64,0x6b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xf7,0xf7,0xf7,0xff,0x5a,0x5a,0x61,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xdd,0x13,0x13,0x20,0x28,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0xd,0xd,0x1b,0x13,0x14,0x14,0x1e,0xdc,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x46,0x46,0x4e,0xff,
    0xf5,0xf5,0xf5,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0x64,0x64,0x6b,0xff,0x64,0x64,0x6b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf7,0xf7,0xf7,0xff,0x49,0x49,0x51,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x13,0x13,0x1e,0xdf,0x18,0x18,0x18,0x15,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x33,0x5,0x14,0x14,0x1e,0xbe,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x1f,0x1f,0x29,0xff,0xe7,0xe7,0xe8,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x64,0x64,0x6b,0xff,0x64,0x64,0x6b,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xe6,0xe6,0xe7,0xff,0x1e,0x1e,0x28,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xc2,0x0,0x0,0x2b,0x6,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x14,0x14,0x1d,0x82,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x8d,0x8d,0x92,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x64,0x64,0x6b,0xff,
    0x64,0x64,0x6b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x8b,0x8b,0x90,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0x7f,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x18,0x18,0x20,0x20,
    0x14,0x14,0x1e,0xf8,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x24,0x24,0x2d,0xff,0xf6,0xf6,0xf6,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xfc,0xfc,0xfc,0xff,0x52,0x52,0x59,0xff,0x53,0x53,0x5a,0xff,0xfc,0xfc,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf6,0xf6,0xf6,0xff,
    0x24,0x24,0x2d,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xf7,0x12,0x12,0x1a,0x1d,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x15,0x15,0x1d,0x94,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x73,0x73,0x79,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x6c,0x6c,0x73,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x6d,0x6d,0x74,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x71,0x71,0x77,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x13,0x13,0x1e,0x92,0x0,0x0,0x0,0x0,0x1c,0x1c,0x1c,0x9,0x14,0x14,0x1e,0xf0,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0xb3,0xb3,0xb6,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xda,0xda,0xdc,0xff,0x15,0x15,0x1f,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x15,0x15,0x1f,0xff,0xdc,0xdc,0xdd,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xb2,0xb2,0xb5,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xf1,0x1c,0x1c,0x1c,0x9,0x13,0x13,0x1f,0x51,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0xdf,0xdf,0xe0,0xff,0x99,0x99,0x9d,0xff,0xd0,0xd0,0xd2,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xb3,0xb3,0xb7,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0xb3,0xb3,0xb6,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xce,0xce,0xd0,0xff,0x96,0x96,0x9a,0xff,
    0xdd,0xdd,0xde,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1d,0x4e,
    0x15,0x15,0x1d,0x93,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x18,0x18,0x22,0xff,0xd6,0xd6,0xd8,0xff,
    0x14,0x14,0x1e,0xff,0x16,0x16,0x20,0xff,0x76,0x76,0x7c,0xff,0xf5,0xf5,0xf5,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xbe,0xbe,0xc1,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0xbe,0xbe,0xc0,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf4,0xf4,0xf4,0xff,0x74,0x74,0x7a,0xff,
    0x15,0x15,0x1f,0xff,0x14,0x14,0x1e,0xff,0xd0,0xd0,0xd2,0xff,0x18,0x18,0x22,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x13,0x13,0x1e,0x90,0x14,0x14,0x1e,0xc2,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x24,0x24,0x2d,0xff,0xe2,0xe2,0xe4,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x35,0x35,0x3e,0xff,
    0xc6,0xc6,0xc8,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe6,0xe6,0xe7,0xff,0x16,0x16,0x20,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x16,0x16,0x20,0xff,0xe7,0xe7,0xe8,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc7,0xc7,0xc9,0xff,
    0x34,0x34,0x3d,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0xde,0xde,0xdf,0xff,0x24,0x24,0x2d,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1d,0xbf,0x14,0x14,0x1e,0xe3,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x29,0x29,0x32,0xff,0xff,0xff,0xff,0xff,0x48,0x48,0x4f,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x52,0x52,0x59,0xff,0xb5,0xb5,0xb8,0xff,0xfd,0xfd,0xfd,0xff,
    0x4f,0x4f,0x56,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x50,0x50,0x57,0xff,0xfd,0xfd,0xfd,0xff,0xb3,0xb3,0xb7,0xff,
    0x52,0x52,0x59,0xff,0x15,0x15,0x1f,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x44,0x44,0x4c,0xff,
    0xff,0xff,0xff,0xff,0x29,0x29,0x32,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1d,0xe1,
    0x14,0x14,0x1e,0xf1,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x23,0x23,0x2c,0xff,0xff,0xff,0xff,0xff,
    0xb3,0xb3,0xb6,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x62,0x62,0x69,0xff,0xc6,0xc6,0xc8,0xff,0x15,0x15,0x1f,0xff,0x15,0x15,0x1f,0xff,0xc7,0xc7,0xc9,0xff,
    0x6c,0x6c,0x72,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0xae,0xae,0xb1,0xff,0xff,0xff,0xff,0xff,0x20,0x20,0x29,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xf0,0x14,0x14,0x1e,0xf1,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x1a,0x1a,0x23,0xff,0xff,0xff,0xff,0xff,0xd5,0xd5,0xd6,0xff,0x3f,0x3f,0x47,0xff,0x96,0x96,0x9a,0xff,0x44,0x44,0x4c,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x3d,0x3d,0x46,0xff,0xff,0xff,0xff,0xff,0x58,0x58,0x5f,0xff,
    0x57,0x57,0x5e,0xff,0xff,0xff,0xff,0xff,0x47,0x47,0x4f,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x47,0x47,0x4f,0xff,0x97,0x97,0x9b,0xff,0x41,0x41,0x49,0xff,0xce,0xce,0xd0,0xff,0xff,0xff,0xff,0xff,0x17,0x17,0x21,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xf0,0x14,0x14,0x1e,0xe3,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0xee,0xee,0xef,0xff,0xca,0xca,0xcd,0xff,0xe6,0xe6,0xe7,0xff,
    0xff,0xff,0xff,0xff,0xfd,0xfd,0xfd,0xff,0xaa,0xaa,0xae,0xff,0x42,0x42,0x4a,0xff,0x1d,0x1d,0x27,0xff,0xa2,0xa2,0xa6,0xff,
    0xff,0xff,0xff,0xff,0x65,0x65,0x6c,0xff,0x64,0x64,0x6b,0xff,0xff,0xff,0xff,0xff,0x73,0x73,0x79,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x80,0x80,0x85,0xff,0xfd,0xfd,0xfd,0xff,0xff,0xff,0xff,0xff,0xe9,0xe9,0xea,0xff,0xc6,0xc6,0xc8,0xff,
    0xef,0xef,0xf0,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1d,0xe1,
    0x14,0x14,0x1e,0xc2,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0xd5,0xd5,0xd6,0xff,
    0xfb,0xfb,0xfb,0xff,0xf4,0xf4,0xf4,0xff,0x9b,0x9b,0xa0,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf5,0xf5,0xf5,0xff,0x3d,0x3d,0x46,0xff,0x3d,0x3d,0x45,0xff,0xf5,0xf5,0xf5,0xff,
    0xc0,0xc0,0xc3,0xff,0x14,0x14,0x1e,0xff,0x17,0x17,0x21,0xff,0x49,0x49,0x50,0xff,0x43,0x43,0x4b,0xff,0xa5,0xa5,0xa9,0xff,
    0xff,0xff,0xff,0xff,0xfb,0xfb,0xfb,0xff,0xd0,0xd0,0xd2,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1d,0xbf,0x13,0x13,0x1e,0x92,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0xaf,0xaf,0xb2,0xff,0xff,0xff,0xff,0xff,0xcf,0xcf,0xd1,0xff,0x14,0x14,0x1e,0xff,0x7d,0x7d,0x83,0xff,
    0xf8,0xf8,0xf8,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x5f,0x5f,0x65,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x60,0x60,0x66,0xff,0xfe,0xfe,0xfe,0xff,0x44,0x44,0x4c,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x16,0x16,0x20,0xff,0xd7,0xd7,0xd9,0xff,0xff,0xff,0xff,0xff,0xb1,0xb1,0xb4,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x13,0x13,0x1e,0x90,0x13,0x13,0x1f,0x51,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x82,0x82,0x87,0xff,0xff,0xff,0xff,0xff,0xb4,0xb4,0xb8,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x26,0x26,0x30,0xff,0x79,0x79,0x7f,0xff,0xfd,0xfd,0xfd,0xff,0xda,0xda,0xdc,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0xdb,0xdb,0xdd,0xff,0xcf,0xcf,0xd1,0xff,
    0x1a,0x1a,0x24,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0xa2,0xa2,0xa6,0xff,0xff,0xff,0xff,0xff,
    0x83,0x83,0x88,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0x4d,
    0x20,0x20,0x20,0x8,0x14,0x14,0x1e,0xf0,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x46,0x46,0x4e,0xff,
    0xff,0xff,0xff,0xff,0xa4,0xa4,0xa8,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x15,0x15,0x1f,0xff,0xc3,0xc3,0xc6,0xff,
    0xff,0xff,0xff,0xff,0xe8,0xe8,0xe9,0xff,0x2a,0x2a,0x33,0xff,0x1a,0x1a,0x23,0xff,0x1a,0x1a,0x23,0xff,0x2b,0x2b,0x34,0xff,
    0xe9,0xe9,0xea,0xff,0xff,0xff,0xff,0xff,0xa5,0xa5,0xa9,0xff,0x15,0x15,0x1f,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x9b,0x9b,0x9f,0xff,0xff,0xff,0xff,0xff,0x44,0x44,0x4c,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xf0,0x1c,0x1c,0x1c,0x9,0x0,0x0,0x0,0x0,0x15,0x15,0x1d,0x94,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x16,0x16,0x20,0xff,0xe9,0xe9,0xea,0xff,0xa7,0xa7,0xaa,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x59,0x59,0x60,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x65,0x65,0x6c,0xff,
    0x64,0x64,0x6b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x72,0x72,0x78,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0xb9,0xb9,0xbc,0xff,0xe7,0xe7,0xe8,0xff,0x15,0x15,0x1f,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1f,0x8d,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x10,0x10,0x21,0x1f,
    0x14,0x14,0x1e,0xf7,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x94,0x94,0x99,0xff,0xbc,0xbc,0xbf,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0xae,0xae,0xb1,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0x65,0x65,0x6c,0xff,0x64,0x64,0x6b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xda,0xda,0xdc,0xff,0x14,0x14,0x1e,0xff,0x17,0x17,0x21,0xff,0xed,0xed,0xee,0xff,0x91,0x91,0x96,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xef,0xd,0xd,0x1b,0x13,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x14,0x14,0x1e,0x80,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x32,0x32,0x3b,0xff,0xdb,0xdb,0xdd,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0xd7,0xd7,0xd9,0xff,0xff,0xff,0xff,0xff,
    0xad,0xad,0xb0,0xff,0x62,0x62,0x69,0xff,0x78,0x78,0x7e,0xff,0x44,0x44,0x4c,0xff,0x43,0x43,0x4b,0xff,0x78,0x78,0x7e,0xff,
    0x62,0x62,0x69,0xff,0xae,0xae,0xb1,0xff,0xff,0xff,0xff,0xff,0xf5,0xf5,0xf5,0xff,0x14,0x14,0x1e,0xff,0x5b,0x5b,0x62,0xff,
    0xfa,0xfa,0xfb,0xff,0x30,0x30,0x38,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x13,0x13,0x1e,0x76,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x33,0x5,0x14,0x14,0x1e,0xbe,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x9b,0x9b,0x9f,0xff,0x2d,0x2d,0x36,0xff,0x14,0x14,0x1e,0xff,
    0xd7,0xd7,0xd9,0xff,0xe2,0xe2,0xe4,0xff,0x18,0x18,0x22,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x18,0x18,0x22,0xff,0xe3,0xe3,0xe5,0xff,0xe1,0xe1,0xe2,0xff,
    0x14,0x14,0x1e,0xff,0xc1,0xc1,0xc4,0xff,0x99,0x99,0x9d,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xca,0x0,0x0,0x2b,0x6,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x1a,0x1a,0x1a,0x14,0x14,0x14,0x1e,0xdd,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x1d,0x1d,0x27,0xff,
    0x59,0x59,0x60,0xff,0x14,0x14,0x1e,0xff,0xb7,0xb7,0xba,0xff,0xb0,0xb0,0xb3,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0xb1,0xb1,0xb4,0xff,0xb0,0xb0,0xb3,0xff,0x60,0x60,0x67,0xff,0xe2,0xe2,0xe3,0xff,0x23,0x23,0x2c,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xe3,0x12,0x12,0x1a,0x1d,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x13,0x13,0x20,0x28,0x14,0x14,0x1e,0xdd,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x22,0x22,0x2b,0xff,0x14,0x14,0x1e,0xff,0x6c,0x6c,0x72,0xff,0xbd,0xbd,0xbf,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0xbe,0xbe,0xc0,0xff,0xab,0xab,0xaf,0xff,0xe4,0xe4,0xe5,0xff,0x46,0x46,0x4e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xe3,0x13,0x13,0x1f,0x29,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0xd,0xd,0x1b,0x13,0x14,0x14,0x1d,0xbf,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x19,0x19,0x22,0xff,0xc6,0xc6,0xc8,0xff,0x18,0x18,0x22,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x19,0x19,0x22,0xff,0xed,0xed,0xee,0xff,0xf2,0xf2,0xf3,0xff,
    0x51,0x51,0x58,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1f,0xc8,0x12,0x12,0x1b,0x1c,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x33,0x5,0x14,0x14,0x1e,0x7f,
    0x14,0x14,0x1e,0xf7,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x3f,0x3f,0x47,0xff,0x52,0x52,0x59,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x5d,0x5d,0x64,0xff,
    0xcb,0xcb,0xce,0xff,0x3b,0x3b,0x43,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xef,0x13,0x13,0x1e,0x76,0x0,0x0,0x33,0x5,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x12,0x12,0x1a,0x1d,0x13,0x13,0x1e,0x92,0x14,0x14,0x1e,0xf0,0x14,0x14,0x1e,0xff,
    0x25,0x25,0x2e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,0x14,0x14,0x1e,0xff,
    0x14,0x14,0x1e,0xff,0x3f,0x3f,0x47,0xff,0x15,0x15,0x1f,0xff,0x14,0x14,0x1e,0xef,0x14,0x14,0x1f,0x8d,0xd,0xd,0x1b,0x13,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x20,0x20,0x20,0x8,0x14,0x14,0x1d,0x4e,0x13,0x13,0x1e,0x91,0x14,0x14,0x1d,0xbf,0x14,0x14,0x1e,0xe2,0x14,0x14,0x1e,0xef,
    0x14,0x14,0x1e,0xef,0x14,0x14,0x1d,0xe1,0x14,0x14,0x1d,0xbf,0x13,0x13,0x1e,0x90,0x14,0x14,0x1e,0x4d,0x20,0x20,0x20,0x8,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
};
