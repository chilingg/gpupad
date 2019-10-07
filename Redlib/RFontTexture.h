#ifndef RFONTTEXTURE_H
#define RFONTTEXTURE_H

#include <RTexture.h>
#include <ft2build.h>
#include FT_FREETYPE_H

class RFontTexture : public RTexture
{
public:
    RFontTexture();
    RFontTexture(const FT_Face &ftFace);
    RFontTexture& operator=(const RFontTexture &fontTex);

    bool generate(const FT_Face &ftFace);

    int bearingX() const;
    int bearingY() const;
    int advance() const;

protected:
    int bearingX_ = 0;
    int bearingY_ = 0;
    int advance_ = 0;
};

inline int RFontTexture::bearingX() const
{
    return bearingX_;
}

inline int RFontTexture::bearingY() const
{
    return bearingY_;
}

inline int RFontTexture::advance() const
{
    return advance_;
}

#endif // RFONTTEXTURE_H
