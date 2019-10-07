#ifndef RFONT_H
#define RFONT_H

#include <RResource.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <RDebug.h>
#include <memory>
#include <RFontTexture.h>

class RFont : public RResource
{
public:
    RFont();
    RFont& operator=(const RFont& font);
    ~RFont() override;

    bool isValid() const override;
    RFontTexture getFontTexture(wchar_t t);
    bool loadFont(const char *path);
    bool loadFont(const std::string &path);
    void setFontSize(unsigned size);

protected:
    static FT_Library ft;
    static int count;

    std::shared_ptr<FT_Face> ftFace_;
    unsigned fontSize_ = 24;
};

inline bool RFont::isValid() const
{
    return ftFace_.get();
}

inline bool RFont::loadFont(const std::string &path)
{
    return loadFont(path.c_str());
}

inline void RFont::setFontSize(unsigned size)
{
    fontSize_ = size;
    if(ftFace_)
        FT_Set_Pixel_Sizes(*ftFace_, 0, fontSize_);//宽度动态计算
}

#endif // RFONT_H
