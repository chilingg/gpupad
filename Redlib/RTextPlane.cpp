#include "RTextPlane.h"

#include "RDebug.h"

RTextPlane::RTextPlane(RShaderProgram *program):
    RTextPlane(L"Text label", 32, 32, "Label", RPoint(0, 0), program)
{

}

RTextPlane::RTextPlane(const RTextPlane &label):
    RPlane(label),
    texts_(label.texts_),
    textImage_(label.textImage_),
    font_(label.font_),
    lineSpacing_(label.lineSpacing_),
    wordSpacing_(label.wordSpacing_)
{

}

RTextPlane::RTextPlane(std::wstring texts, int width, int height, const std::string &name, RPoint pos, RShaderProgram *program):
    RPlane(width, height, name, pos, program),
    texts_(texts)
{
    textImage_.load(width, height, 4, nullptr);
    setAlignment(RPlane::Align_Left, RPlane::Align_Top);
    flipV();
}

RTextPlane::~RTextPlane()
{

}

void RTextPlane::setPadding(int top, int bottom, int left, int right)
{
    top = top > 0 ? top : 0;
    bottom = bottom > 0 ? bottom : 0;
    left = left > 0 ? left : 0;
    right = right > 0 ? right : 0;
    RPlane::setPadding(top, bottom, left, right);
}

void RTextPlane::setPadding(int value)
{
    value = value > 0 ? value : 0;
    RPlane::setPadding(value);
}

void RTextPlane::setTexts(std::wstring texts)
{
    texts_.swap(texts);
    updateModelMat();
}

void RTextPlane::setFontSize(unsigned size)
{
    font_.setFontSize(size);
    updateModelMat();
}

bool RTextPlane::setFont(RFont font)
{
#ifdef R_DEBUG
    if(!font.isValid()) return false;
#endif
    font_.swap(font);
    updateModelMat();

    return true;
}

void RTextPlane::setlineSpacing(float value)
{
    lineSpacing_ = value;
    updateModelMat();
}

void RTextPlane::setWordSpacing(float value)
{
    wordSpacing_ = value;
    updateModelMat();
}

void RTextPlane::updateModelMatNow()
{
    RPlane::updateModelMatNow();

    int templ = paddingLeft(), tempr = paddingRight(), tempt = paddingTop(), tempb = paddingBottm();
    setPadding(0);
    RPlane::updateModelMatNow();
    if(textImage_.width() != width() || textImage_.height() != height())
        textImage_.load(width(), height(), 4, nullptr);
    setPadding(tempt, tempb, templ, tempr);

    int lineSize = static_cast<int>(font_.size()*lineSpacing_+0.5f);//换行
    int lengthMax = innerWidth();
    int lineMax = innerHeight();
    int linepos = paddingLeft();

    //计算每一行的长度
    int lineWidth = 0;
    std::vector<int> lineWidths;
    for(size_t i = 0; i < texts_.size(); ++i)
    {
        lineWidth += static_cast<int>(font_.getFontGlyph(texts_[i]).advance * wordSpacing_+0.5f);//+0.5四舍五入
        //RDebug() << lineWidth << (texts_[i] == L'\n') << (lineWidth > lengthMax) << (i+1 == texts_.size());
        if(texts_[i] == L'\n')
        {
            lineWidths.push_back(lineWidth - static_cast<int>(font_.getFontGlyph(texts_[i]).advance*wordSpacing_+0.5f));
            lineWidth = 0;//忽视换行符
            linepos += lineSize;

            if(linepos + static_cast<int>(font_.size()) + static_cast<int>(font_.size()*0.3+0.5) > lineMax) break;//预留三分之一空间
        }

        if(lineWidth > lengthMax)
        {
            lineWidths.push_back(lineWidth - static_cast<int>(font_.getFontGlyph(texts_[i]).advance*wordSpacing_+0.5f));
            lineWidth = static_cast<int>(font_.getFontGlyph(texts_[i]).advance * wordSpacing_+0.5f);//+0.5四舍五入
            linepos += lineSize;

            if(linepos + static_cast<int>(font_.size()) + static_cast<int>(font_.size()*0.3+0.5) > lineMax) break;//预留三分之一空间
        }

        if(i+1 == texts_.size())
        {
            lineWidths.push_back(lineWidth);
            break;
        }
    }

    lengthMax = width();
    lineMax = height();

    if(vAlign() == Align_Bottom) linepos = lineMax - static_cast<int>(lineWidths.size()) * lineSize - paddingBottm();
    else if(vAlign() == Align_Mind) linepos = (lineMax - static_cast<int>(lineWidths.size()) * lineSize) / 2;
    else linepos = paddingTop();//Align_Top 缺省默认

    unsigned char *data = textImage_.data();
    int wordOffset;
    RFontGlyph glyph;
    size_t textNum = 0;
    for(auto linew : lineWidths)
    {
        if(hAlign() == Align_Right) wordOffset = lengthMax - linew - paddingRight();
        else if(hAlign() == Align_Mind) wordOffset = (lineMax - linew) / 2;
        else wordOffset = paddingLeft();//Align_Top 缺省默认

        RDebug() << "Line width:" << linew;
        while(textNum < texts_.size())
        {
            if(texts_[textNum] == L'\n')  { ++textNum; continue; }

            glyph = font_.getFontGlyph(texts_[textNum]);
            int startx = wordOffset + glyph.bearingX;
            int starty = linepos + static_cast<int>(font_.size()) - glyph.bearingY;
            const unsigned char* bitmap = glyph.data.get();
            RDebug() << "startx:" << startx << "starty:" << starty;
            for(int y = 0; y < glyph.height; ++y)
            {
                for(int x = 0; x < glyph.width; ++x)//, ++starty)
                {
                    if(starty+y < 0 || startx+x < 0 || startx+x > lengthMax) continue;

                    //RDebug() << y*glyph.width+x+starty*lengthMax+startx;
                    data[((starty+y)*lengthMax+startx+x)*4] = 0xff;
                    data[((starty+y)*lengthMax+startx+x)*4+1] = 0xff;
                    data[((starty+y)*lengthMax+startx+x)*4+2] = 0xff;
                    data[((starty+y)*lengthMax+startx+x)*4+3] = bitmap[y*glyph.width+x];
                    //RDebug() << (starty+y)*lengthMax << startx+x << y*glyph.width+x << static_cast<unsigned>(bitmap[y*glyph.width+x]);
                }
            }
            RDebug() << "text:" << texts_[textNum] << " w:" << glyph.width << "h:" << glyph.height
                     << "advance:" << glyph.advance << "beaningX:" << glyph.bearingX
                     << "beaningY:" << glyph.bearingY << "offset:" << wordOffset;
            ++textNum;
            if(wordOffset + (glyph.advance * wordSpacing_+0.5f) >= linew) { linepos += lineSize; break; }
            else wordOffset += static_cast<int>(glyph.advance * wordSpacing_+0.5f);
        }
    }
    texture_.generate(textImage_);
    updateModelMatOver();
}
