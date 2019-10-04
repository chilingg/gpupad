#include "RFont.h"

FT_Library RFont::ft;
int RFont::count = 0;

RFont::RFont()
{
    if(count == 0)
    {
        if(FT_Init_FreeType((&ft)))
            printErro("ERROR::FREETYPE: Could not init FreeType Library");
    }
    ++count;
}

RFont::~RFont()
{
    FT_Done_Face(ftFace_);
    --count;
    if(count == 0)
        FT_Done_FreeType(ft);
}
