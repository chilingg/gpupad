#include "RFont.h"

FT_Library RFont::ft;
int RFont::count = 0;

RFont::RFont():
    RResource(),
    ftFace_()
{
    if(count == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//禁用字节对齐限制
        if(FT_Init_FreeType((&ft)))
            printErro("ERROR::FREETYPE: Could not init FreeType Library");
    }
    ++count;
}

RFont &RFont::operator=(const RFont &font)
{
    RFont temp(*this);
    ftFace_ = font.ftFace_;
    fontSize_ = font.fontSize_;
    return *this;
}

RFont::~RFont()
{
    if(ftFace_ && ftFace_.unique())
        FT_Done_Face(*ftFace_);

    --count;
    if(count == 0)
        FT_Done_FreeType(ft);
}

RFontTexture RFont::getFontTexture(wchar_t t)
{
    RFontTexture tex;
    if(ftFace_)
    {
        if(FT_Load_Char(*ftFace_, t, FT_LOAD_RENDER))
            printErro("ERROR::FREETYTPE: Failed to load Glyph");
        else
            tex.generate(*ftFace_);
    }
    return tex;
}

bool RFont::loadFont(const char *path)
{
    auto rePath = checkResourcePath(path);
    if(rePath.empty())
        return false;

    if(!ftFace_.unique() || !ftFace_)
        ftFace_ = std::make_shared<FT_Face>();
    else if(ftFace_)
        FT_Done_Face(*ftFace_);

    if(FT_New_Face(ft, path, 0, ftFace_.get()))
    {
        printErro("ERROR::FREETYPE: Failed to load font");
        ftFace_.reset();
    }

    //宽度动态计算
    FT_Set_Pixel_Sizes(*ftFace_, 0, fontSize_);

    return ftFace_.get();
}
