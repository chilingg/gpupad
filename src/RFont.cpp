#include "rsc/RFont.h"
#include "RDebug.h"

#include <fstream>

using namespace Redopera;

unsigned RFont::cacheMaxSize_ = 1000;

void RFont::setCasheSize(unsigned size)
{
    cacheMaxSize_ = size;
}

void RFont::setDefaultFontSize(unsigned size)
{
    defaultFont.setSize(size);
}

void RFont::setDefaultFont(RFont font)
{
    defaultFont = font;
}

RFont::RFont():
    RResource("Font", typeid(this).name()),
    resource_(defaultFont.resource_)
{

}

RFont::RFont(const std::string &path, const std::string &name, unsigned fsize):
    RResource(name, typeid(this).name()),
    size_(fsize)
{
    load(path);
}

RFont::RFont(const std::shared_ptr<RData[]> &data, const std::string &name, unsigned fsize):
    RResource(name, typeid(this).name()),
    size_(fsize)
{
    load(data);
}

RFont::RFont(const RFont &font):
    RResource(font),
    resource_(font.resource_),
    caches_(font.caches_),
    size_(font.size_)
{

}

RFont::RFont(const RFont &&font):
    RResource(std::move(font)),
    resource_(std::move(font.resource_)),
    caches_(std::move(font.caches_)),
    size_(font.size_)
{

}

RFont &RFont::operator=(RFont font)
{
    swap(font);
    return *this;
}

void RFont::swap(RFont &font)
{
    RResource::swap(font);
    resource_.swap(font.resource_);
    caches_.swap(font.caches_);
    using std::swap;
    swap(size_, font.size_);
}

bool RFont::isValid() const
{
    return resource_ != nullptr;
}

unsigned RFont::size() const
{
    return size_;
}

void RFont::setSize(unsigned size)
{
    size_ = size;
    caches_ = std::make_shared<std::map<RChar, Glyph>>();
}

RFont::Glyph *RFont::getFontGlyph(RFont::RChar c)
{
    Glyph *glyph = &(*caches_)[c];
    if(!glyph->data)
    {
        glyph->data.reset(stbtt_GetCodepointBitmap(
                    &resource_->info, 0, stbtt_ScaleForPixelHeight(&resource_->info, size_),
                    c, &glyph->width, &glyph->height, &glyph->xoff, &glyph->yoff));

        if(caches_->size() > cacheMaxSize_)
            clearFontDataCache();
    }

    return glyph;
}

bool RFont::load(const std::string &path)
{
    std::string newpath = rscpath(path);

    RData *data;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(newpath, std::ios::binary | std::ios::ate);
        size_t size = file.tellg();
        data = new RData[size];
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char*>(data), size);
        file.close();
    }
    catch(...)
    {
        prError("Failed to load font <" + name() + "> in " + newpath);
        return false;
    }

    resource_ = std::make_shared<FontResource>();
    stbtt_InitFont(&resource_->info, data, 0);

    if(check(resource_->info.numGlyphs == 0, "Unknow font file <" + name() + "> in " + newpath))
    {
        resource_.reset();
        return false;
    }

    std::shared_ptr<int[]> sp3(new int[10]());
    resource_->font.reset(data);
    caches_ = std::make_shared<std::map<RChar, Glyph>>();
    return true;
}

bool RFont::load(const std::shared_ptr<RData[]> &data)
{
    resource_ = std::make_shared<FontResource>();
    stbtt_InitFont(&resource_->info, data.get(), 0);

    if(check(resource_->info.numGlyphs == 0, "Unknow font file <" + name() + '>'))
    {
        resource_.reset();
        return false;;
    }

    resource_->font = data;
    caches_ = std::make_shared<std::map<RChar, Glyph>>();
    return true;
}

void RFont::release()
{
    resource_.reset();
    caches_.reset();
}

void RFont::clearFontDataCache()
{
    *caches_ = std::map<RChar, Glyph>();
}

void swap(RFont &font1, RFont &font2)
{
    font1.swap(font2);
}
