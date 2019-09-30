#include "RTextline.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <constant.h>

RTextline::RTextline(int width, int height):
    RTexObject(width, height)
{
    //初始化FreeType
    FT_Library ft;
    if(FT_Init_FreeType((&ft)))
        printErro("ERROR::FREETYPE: Could not init FreeType Library");

    //字体加载至Face
    FT_Face ftFace;
    std::string fontPath = RE_PATH+"fonts/SourceHanSerifSC_EL-M/SourceHanSerifSC-Regular.otf";
    if(FT_New_Face(ft, fontPath.c_str(), 0, &ftFace))
        printErro("ERROR::FREETYPE: Failed to load font");

    //字体宽高，宽为0表示从给定高度动态计算
    FT_Set_Pixel_Sizes(ftFace, 0, 255);

    //加载字符
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//禁用字节对齐限制
    //加载字符的字形
    if(FT_Load_Char(ftFace, L'R', FT_LOAD_RENDER))
        printErro("ERROR::FREETYTPE: Failed to load Glyph");

    std::string name = "c";
    textures["c"].generate(ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows, ftFace->glyph->bitmap.buffer, 1);
    setCurrentTexture(name);

    //清理FreeType的资源
    FT_Done_Face(ftFace);
    FT_Done_FreeType(ft);

    flipV(true);
}

void RTextline::renderControl(RShaderProgram *shader)
{
    RTexObject::renderControl(shader);
}
