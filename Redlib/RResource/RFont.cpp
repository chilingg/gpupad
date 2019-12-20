#include "RFont.h"
#include "RDebug.h"

FT_Library RFont::ftLibrary;
int RFont::count = 0;
unsigned RFont::cacheSize_ = 512;

void RFont::setCasheSize(unsigned size)
{
    cacheSize_ = size;
}

RFont::RFont():
    RResource("UnnameFont")
{
    if(++count == 1)
        if(printError(FT_Init_FreeType(&ftLibrary), "Could not init FreeType Library!"))
            throw("Font library initialization failed");

    setAsDefualtFont(fontSize_);
}

RFont::RFont(const std::string &path, const std::string &name):
    RResource(name)
{
    if(++count == 1)
        if(printError(FT_Init_FreeType(&ftLibrary), "Could not init FreeType Library!"))
            throw("Font library initialization failed");

    loadFont(path, fontSize_);
}

RFont::RFont(const RFont &font):
    RResource(font),
    fontData_(font.fontData_),
    ftFace_(font.ftFace_),
    fontFilePath_(font.fontFilePath_),
    fontSize_(font.fontSize_)
{
    ++count;
}

RFont::RFont(const RFont &&font):
    RResource(std::move(font)),
    fontData_(font.fontData_),
    ftFace_(font.ftFace_),
    fontFilePath_(font.fontFilePath_),
    fontSize_(font.fontSize_)
{
    ++count;
}

RFont &RFont::operator=(RFont font)
{
    swap(font);
    return *this;
}

void RFont::swap(RFont &font)
{
    RResource::swap(font);
    using std::swap;
    swap(fontData_, font.fontData_);
    swap(ftFace_, font.ftFace_);
    swap(fontFilePath_, font.fontFilePath_);
    swap(fontSize_, font.fontSize_);
}

RFont::~RFont()
{
    if(--count == 0)
    {
        ftFace_.reset();
        FT_Done_FreeType(ftLibrary);
    }
}

bool RFont::isValid() const
{
    return ftFace_ != nullptr;
}

unsigned RFont::size() const
{
    return fontSize_;
}

RFontGlyph RFont::getFontGlyph(wchar_t c)
{
    if(fontData_->count(c))
        return (*fontData_)[c];

    if(fontData_->size() > cacheSize_)
        clearFontDataCache();

    RFontGlyph fd;
#ifdef R_DEBUG
    if(printError(ftFace_ == nullptr, "Failed to get font data from " + nameID() + ", Its null!"))
        return fd;
#endif

    assert(!FT_Load_Char(*ftFace_, static_cast<FT_ULong>(c), FT_LOAD_RENDER));
    fd.width = (*ftFace_)->glyph->bitmap.width;
    fd.height = (*ftFace_)->glyph->bitmap.rows;
    fd.bearingX = (*ftFace_)->glyph->bitmap_left;
    fd.bearingY = (*ftFace_)->glyph->bitmap_top;
    fd.advance = static_cast<unsigned>((*ftFace_)->glyph->advance.x >> 6);

    size_t size = fd.width * fd.height;
    fd.data.reset(new unsigned char[size], [](unsigned char *p){ delete [] p;});
    for(size_t i = 0; i < size; ++i)
        *(fd.data.get()+i) = *((*ftFace_)->glyph->bitmap.buffer + i);

    (*fontData_)[c] = fd;
    return fd;
}

void RFont::setFontSize(unsigned size)
{
    if(fontFilePath_.empty())
    {
        setAsDefualtFont(size);
        return;
    }

    ftFace_.reset(new FT_Face, deleteFontFace);
    assert(!FT_New_Face(ftLibrary, fontFilePath_.c_str(), 0, ftFace_.get()));
    FT_Set_Pixel_Sizes(*ftFace_, 0, size);
    fontSize_ = size;
    fontData_ = std::make_shared<std::map<wchar_t, RFontGlyph>>();
}

void RFont::setAsDefualtFont(unsigned size)
{
    ftFace_.reset(new FT_Face, deleteFontFace);

#ifdef R_DEBUG
    if(printError(FT_New_Memory_Face(ftLibrary, Font_SourceCodePro, Font_FileSize, 0, ftFace_.get()), "Cannot set default font!"))
        throw("Cannot set default font!");
#else
    FT_New_Memory_Face(ftLibrary, defaultFont, size, 0 , face_);
#endif

    FT_Set_Pixel_Sizes(*ftFace_, 0, size);
    fontSize_ = size;
    fontData_ = std::make_shared<std::map<wchar_t, RFontGlyph>>();
    fontFilePath_.clear();
}

bool RFont::loadFont(std::string path, unsigned size)
{
    path = checkFilePath(path);
    if(path.empty())
        return false;

    FT_Face *face = new FT_Face;
    if(printError(FT_New_Face(ftLibrary, path.c_str(), 0, face), "Failed to load font: " + path + " in " + nameID()))
    {
        delete face;
        setAsDefualtFont(size);
        return false;
    }

    ftFace_.reset(face, deleteFontFace);
    //宽度动态计算
    FT_Set_Pixel_Sizes(*ftFace_, 0, size);
    fontSize_ = size;
    fontData_ = std::make_shared<std::map<wchar_t, RFontGlyph>>();
    fontFilePath_.swap(path);

    return true;
}

void RFont::freeFont()
{
    fontData_.reset();
    ftFace_.reset();
    fontFilePath_.clear();
}

void RFont::clearFontDataCache()
{
    std::map<wchar_t, RFontGlyph> temp;
    fontData_->swap(temp);
}

void RFont::deleteFontFace(FT_Face *p)
{
    FT_Done_Face(*p);
    delete p;
}

void swap(RFont &font1, RFont &font2)
{
    font1.swap(font2);
}
