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

    bool setFont();

protected:
    static FT_Library ft;
    static int count;

    FT_Face ftFace;
};

#endif // RFONT_H
