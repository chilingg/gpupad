#include "RFontTexture.h"

RFontTexture::RFontTexture():
    RTexture()
{

}

RFontTexture::RFontTexture(const FT_Face &ftFace):
    RTexture()
{
    generate(ftFace);
}

RFontTexture &RFontTexture::operator=(const RFontTexture &fontTex)
{
    bearingX_ = fontTex.bearingX_;
    bearingY_ = fontTex.bearingY_;
    advance_ = fontTex.advance_;
    RTexture::operator=(fontTex);

    return *this;
}

bool RFontTexture::generate(const FT_Face &ftFace)
{
    bool b = RTexture::generate(ftFace->glyph->bitmap.width,
                                ftFace->glyph->bitmap.rows,
                                ftFace->glyph->bitmap.buffer, 1);

    bearingX_ = ftFace->glyph->bitmap_left;
    bearingY_ = ftFace->glyph->bitmap_top;
    advance_ = ftFace->glyph->advance.x;

    return b;
}
