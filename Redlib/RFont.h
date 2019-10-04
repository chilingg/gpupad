#ifndef RFONT_H
#define RFONT_H

#include <RResource.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <RDebug.h>

class RFont : public RResource
{
public:
    RFont();
    ~RFont();

    bool loadFont(const char *path);
    bool loadFont(const std::string &path);
    void setFontSize(unsigned size);

protected:
    static FT_Library ft;
    static int count;

    FT_Face ftFace_;
    unsigned fontSize_ = 24;
};

inline bool RFont::loadFont(const char *path)
{
    if(FT_New_Face(ft, path, 0, &ftFace_))
    {
        printErro("ERROR::FREETYPE: Failed to load font");
        state = false;
        return state;
    }
    //宽度动态计算
    FT_Set_Pixel_Sizes(ftFace_, 0, fontSize_);
    return state = true;
}

inline bool RFont::loadFont(const std::string &path)
{
    return loadFont(path.c_str());
}

inline void RFont::setFontSize(unsigned size)
{
    fontSize_ = size;
}

#endif // RFONT_H
