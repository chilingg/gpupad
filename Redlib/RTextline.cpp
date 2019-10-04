#include "RTextline.h"

#include <constant.h>

RShaderProgram *RTextline::textProgram(nullptr);

RTextline::RTextline(int width, int height):
    RTexObject(width, height),
    textTexs(),
    texts_(),
    backgroundColor_(0.5f, 0.5f, 0.5f, 1.0f)
{
    if(textProgram == nullptr)
    {
        textProgram = new RShaderProgram;
        RShader vertex(RE_PATH + "shaders/vertex.vert", RShader::VERTEX_SHADER);
        RShader textShader((RE_PATH + "shaders/text.frag"), RShader::FRAGMENT_SHADER);

        textProgram->attachShader(vertex);
        textProgram->attachShader(textShader);
        textProgram->linkProgram();
    }

    //flipV(true);
    setTextureSizePattern(Length);

    //背景图
    unsigned char data[3] = {255, 255, 255};
    textures["Background"].generate(1, 1, data, 3);
    setCurrentTexture("Background");
}

void RTextline::updataSizeMat()
{
    sizeMat = glm::mat4(1);
    float fontTexW = 0;
    float fontTexH = 0;
    for(const auto &t : texts_)
    {
        fontTexW += textTexs[t].width();
        if(fontTexH < textTexs[t].height())
            fontTexH = textTexs[t].height();
    }
    fontTexW /= fontSizeRatio_;
    fontTexH /= fontSizeRatio_;

    if(_hAlign == Align_Left)
        sizeMat[3][0] = _paddingLeft;
    else if(_hAlign == Align_Mind)
        sizeMat[3][0] = _width/2 - fontTexW/2;
    else if(_hAlign == Align_Right)
        sizeMat[3][0] = _width - fontTexW - _paddingRight;
    if(_vAlign == Align_Bottom)
        sizeMat[3][1] = _paddingBottom;
    else if(_vAlign == Align_Mind)
        sizeMat[3][1] = _height/2 - fontTexH/2;
    else if(_vAlign == Align_Top)
        sizeMat[3][1] = _height - fontTexH - _paddingTop;

    sizeMat[3][0] += _marginLeft;
    sizeMat[3][1] += _marginBottom;

    glm::mat4 flipMat(1);
    flipMat[3][1] = 1;
    flipMat[1][1] = -1;
    sizeMat *= flipMat;
}

bool RTextline::loadFontTextures()
{
    //初始化FreeType
    FT_Library ft;
    if(FT_Init_FreeType((&ft)))
    {
        printErro("ERROR::FREETYPE: Could not init FreeType Library");
        return false;
    }

    //字体加载至Face
    FT_Face ftFace;
    std::string fontPath = RE_PATH+"fonts/SourceHanSerifSC_EL-M/SourceHanSerifSC-Regular.otf";
    if(FT_New_Face(ft, fontPath.c_str(), 0, &ftFace))
    {
        printErro("ERROR::FREETYPE: Failed to load font");
        return false;
    }

    //字体宽高，宽为0表示从给定高度动态计算
    FT_Set_Pixel_Sizes(ftFace, 0, fontSize_ * fontSizeRatio_);

    //加载字符
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//禁用字节对齐限制
    //加载字符的字形
    for(const auto &t : texts_)
    {
        if(FT_Load_Char(ftFace, t, FT_LOAD_RENDER))
        {
            printErro("ERROR::FREETYTPE: Failed to load Glyph");
            return false;
        }
        //RDebug() << ftFace->glyph->bitmap.rows << fontSize_ * fontSizeRatio_ << t;
        textTexs[t].generate(ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows, ftFace->glyph->bitmap.buffer, 1);
    }

    //清理FreeType的资源
    FT_Done_Face(ftFace);
    FT_Done_FreeType(ft);

    updataSizeMat();

    return true;
}

void RTextline::render(RShaderProgram *shader)
{
    glBindVertexArray(texVAO);

    //渲染背景
    textures[currentTex].bind();
    //setTextureSizePattern(Auto);
    shader->use();
    glm::mat4 model(1.0f);
    model[3][0] = _pos.x + _marginLeft;
    model[3][1] = _pos.y + _marginBottom;
    model[0][0] = _width;
    model[1][1] = _height;
    glm::mat4 flipMat(1);
    if(_flipH == true)
    {
        flipMat[3][0] = 1;
        flipMat[0][0] = -1;
    }
    if(_flipV == true)
    {
        flipMat[3][1] = 1;
        flipMat[1][1] = -1;
    }
    model *= flipMat;
    shader->setUniformMatrix4fv("model", glm::value_ptr(model));
    shader->setUniform4F("color", backgroundColor_);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    textProgram->use();
    textProgram->setUniform4F("color", color);
    float intervalX = 0;
    float intervalY = 0;
    for(const auto &t : texts_)
    {
        float tw = textTexs[t].width() / fontSizeRatio_;
        float th = textTexs[t].height() / fontSizeRatio_;
        if(tw + intervalX > innerWidth())
        {
            intervalX = 0;
            intervalY -= fontSize_ * rowSpacing;
            //RDebug() << fontSize_ << th;
        }
        model = glm::mat4(1.0f);
        model[3][0] = _pos.x + intervalX + sizeMat[3][0];
        model[3][1] = _pos.y + sizeMat[3][1] + th + intervalY;
        model[0][0] = tw;
        model[1][1] = th * -1;

        textProgram->setUniformMatrix4fv("model", glm::value_ptr(model));
        textTexs[t].bind();

        glDrawArrays(GL_TRIANGLES, 0, 6);
        intervalX += tw;
    }
    glBindVertexArray(0);
}
