#include "RImage.h"
#include "RDebug.h"

RImage::RImage():
    RResource(),
    internalData(std::make_shared<unsigned char*>(nullptr))
{

}

RImage::RImage(const char *path, bool flip):
    RImage(std::string(path), flip)
{

}

RImage::RImage(const std::string &path, bool flip):
    RImage()
{
    auto rePath = checkResourcePath(path);
    load(rePath, flip);
}

RImage &RImage::operator=(const RImage &img)
{
    RImage temp(*this);

    internalData = img.internalData;
    width = img.width;
    height = img.height;
    channel = img.channel;

    return *this;
}

RImage::~RImage()
{
    if(internalData.unique() && *internalData)
        stbi_image_free(*internalData);
}

bool RImage::load(const std::string &path, bool flip)
{
    auto rePath = checkResourcePath(path);
    if(rePath.empty())
        return false;

    if(!internalData.unique())
        internalData = std::make_shared<unsigned char*>(nullptr);
    else if(*internalData)
        stbi_image_free(*internalData);

    stbi_set_flip_vertically_on_load(flip);
    *internalData = stbi_load(path.c_str(), &width, &height, &channel, 0);

    if(!*internalData)
    {
        printErro("Image failed to load at path: ");
        printErro(path);
        stbi_image_free(*internalData);
        *internalData = nullptr;
    }

    return *internalData;
}
