#ifndef RFONT_H
#define RFONT_H

#include "RResource.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <memory>
#include <map>

struct RFontGlyph {
    unsigned width = 0;
    unsigned height = 0;
    int bearingX = 0;
    int bearingY = 0;
    unsigned advance = 0;
    std::shared_ptr<unsigned char> data;
};

class RFont : public RResource
{
    friend void swap(RFont &font1, RFont &font2);

public:
    static void setCasheSize(unsigned size);

    static const unsigned Font_FileSize;
    static const unsigned char Font_SourceCodePro[];

    RFont();
    RFont(const std::string &path, const std::string &name);
    RFont(const RFont &font);
    RFont(const RFont &&font);
    RFont& operator=(RFont font);
    void swap(RFont &font);
    ~RFont();

    bool isValid() const;
    unsigned size() const;
    RFontGlyph getFontGlyph(wchar_t c);

    void setFontSize(unsigned size);
    void setAsDefualtFont(unsigned size);

    bool loadFont(std::string path, unsigned size = 24);
    void freeFont();
    void clearFontDataCache();

private:
    static void deleteFontFace(FT_Face *p);

    static FT_Library ftLibrary;
    static int count;
    static unsigned cacheSize_;

    std::shared_ptr<std::map<wchar_t, RFontGlyph>> fontData_;
    std::shared_ptr<FT_Face> ftFace_;
    std::string fontFilePath_;
    unsigned fontSize_ = 24u;
};

void swap(RFont &font1, RFont &font2);

#endif // RFONT_H
