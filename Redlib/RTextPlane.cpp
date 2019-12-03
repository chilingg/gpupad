#include "RTextPlane.h"

#include "RDebug.h"

RTextPlane::RTextPlane(RShaderProgram *program, const std::string &name):
    RTextPlane(L"Text label", 32, 32, name, RPoint(0, 0), program)
{

}

RTextPlane::RTextPlane(const RTextPlane &label):
    RPlane(label),
    texts_(label.texts_),
    fontColor_(label.fontColor_),
    textImage_(label.textImage_),
    font_(label.font_),
    lineSpacing_(label.lineSpacing_),
    wordSpacing_(label.wordSpacing_),
    typeset_(label.typeset_)
{

}

RTextPlane::RTextPlane(std::wstring texts, int width, int height, const std::string &name, RPoint pos, RShaderProgram *program):
    RPlane(width, height, name, pos, program),
    texts_(texts),
    fontColor_(0xffffffffu)
{
    textImage_.load(width, height, 4, nullptr);
    setAlignment(RPlane::Align_Left, RPlane::Align_Top);
    flipV();
}

RTextPlane::~RTextPlane()
{

}

void RTextPlane::setFontColor(RColor color)
{
    fontColor_ = color;
}

void RTextPlane::setFontColor(unsigned r, unsigned g, unsigned b)
{
    fontColor_ = RColor(r, g, b);
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

void RTextPlane::verticalTypeset()
{
    typeset_ = VerticalTypeset;
}

void RTextPlane::horizontalTypeset()
{
    typeset_ = HorizontalTypeset;
}

void RTextPlane::updateModelMatNow()
{
    RPlane::updateModelMatNow();

    int templ = paddingLeft(), tempr = paddingRight(), tempt = paddingTop(), tempb = paddingBottm();
    setPadding(0);
    RPlane::updateModelMatNow();
    if(textImage_.width() != width() || textImage_.height() != height())
        textImage_.load(width(), height(), 4, nullptr);
    else
        textImage_.full(RColor(0x0u));

    setPadding(tempt, tempb, templ, tempr);

    if(typeset_ == VerticalTypeset)
        verticalTextToTexture();
    else
        horizontalTextToTexture();

    updateModelMatOver();
}

void RTextPlane::verticalTextToTexture()
{
    int lineSize = static_cast<int>(font_.size()*lineSpacing_+0.5f);//换行
    int wordSize = static_cast<int>(font_.size());
    int lengthMax = innerHeight();
    int lineMax = innerWidth();
    int linepos = 0;

    std::vector<int> lineLengths { 0 };
    if(lengthMax > static_cast<int>(font_.size()))
    {
        int lineLength = 0;
        for(size_t i = 0; i < texts_.size(); ++i)
        {
            lineLength += static_cast<int>(wordSize * wordSpacing_+0.5f);
            if(texts_[i] == L'\n')
            {
                if(linepos + static_cast<int>(font_.size()) + static_cast<int>(font_.size()*0.3+0.5) > lineMax) break;

                lineLengths.back() += 1;//换行符占位
                lineLengths.push_back(0);
                lineLength = 0;//忽视换行符
                linepos += lineSize;

                continue;
            }
            if(lineLength > lengthMax)
            {
                linepos += lineSize;
                if(linepos + static_cast<int>(font_.size()) + static_cast<int>(font_.size()*0.3+0.5) > lineMax) break;

                lineLengths.push_back(static_cast<int>(wordSize * wordSpacing_+0.5f));
                lineLength = static_cast<int>(wordSize * wordSpacing_+0.5f);//+0.5四舍五入

                continue;
            }
            lineLengths.back() += static_cast<int>(wordSize* wordSpacing_+0.5f);
        }
    }
    if(lineLengths.back() == 0) lineLengths.pop_back();

    lengthMax = height();
    lineMax = width();

    if(hAlign() == Align_Left) linepos = static_cast<int>(lineLengths.size())*lineSize + paddingLeft();
    else if(hAlign() == Align_Mind) linepos = (lineMax + static_cast<int>(lineLengths.size())*lineSize) / 2;
    else linepos = lineMax - paddingRight();

    unsigned char *data = textImage_.data();
    int wordOffset;
    RFontGlyph glyph;
    size_t textNum = 0;

    for(auto linew : lineLengths)
    {
        if(vAlign() == Align_Bottom) wordOffset = lengthMax - linew - paddingBottm();
        else if(vAlign() == Align_Mind) wordOffset = (lengthMax - linew) / 2;
        else wordOffset = paddingLeft();//Align_Top 缺省默认
        linew += wordOffset;

        //RDebug() << "Line width:" << linew << "height:" << linepos << "offset:" << wordOffset;
        while(textNum < texts_.size())
        {
            if(texts_[textNum] == L'\n')
            {
                linepos -= lineSize;
                ++textNum;
                break;
            }
            glyph = font_.getFontGlyph(texts_[textNum]);
            int startx = linepos - (static_cast<int>(font_.size() + glyph.width))/2;
            int starty = wordOffset + static_cast<int>(font_.size()) - glyph.bearingY;
            const unsigned char* bitmap = glyph.data.get();
            unsigned char r = fontColor_.r();
            unsigned char g = fontColor_.g();
            unsigned char b = fontColor_.b();
            //RDebug() << "startx:" << startx << "starty:" << starty;
            for(int y = 0; y < glyph.height; ++y)
            {
                for(int x = 0; x < glyph.width; ++x)
                {
                    if(starty+y < 0 || startx+x < 0 || starty+y > lengthMax) continue;

                    data[((starty+y)*lineMax+startx+x)*4] = r;
                    data[((starty+y)*lineMax+startx+x)*4+1] = g;
                    data[((starty+y)*lineMax+startx+x)*4+2] = b;
                    data[((starty+y)*lineMax+startx+x)*4+3] = bitmap[y*glyph.width+x];
                }
            }
            //RDebug() << "text:" << texts_[textNum] << " w:" << glyph.width << "h:" << glyph.height
                     //<< "advance:" << glyph.advance << "beaningX:" << glyph.bearingX
                     //<< "beaningY:" << glyph.bearingY << "offset:" << wordOffset;
            ++textNum;
            if(wordOffset + (wordSize * wordSpacing_+0.5f) >= linew) { linepos -= static_cast<int>(font_.size()*lineSpacing_+0.5f); break; }
            else wordOffset += static_cast<int>(wordSize * wordSpacing_+0.5f);
        }
    }
    //若不能显示所有字符，在末尾添加一条省略线
    if(textNum < texts_.size())
    {
        linepos = paddingLeft() + (innerWidth() > 0 ? 1 : 0);
        wordOffset = paddingTop();
        for(int i = 0; i < innerHeight(); ++i)
        {
            data[((i+wordOffset)*lineMax + linepos)*4] = 0xff;
            data[((i+wordOffset)*lineMax + linepos)*4 + 1] = 0;
            data[((i+wordOffset)*lineMax + linepos)*4 + 2] = 0;
            data[((i+wordOffset)*lineMax + linepos)*4 + 3] = 0xff;
        }
    }
    texture_.generate(textImage_);
}

void RTextPlane::horizontalTextToTexture()
{
    int lineSize = static_cast<int>(font_.size()*lineSpacing_+0.5f);//换行
    int lengthMax = innerWidth();
    int lineMax = innerHeight();
    int linepos = 0;

    //计算每一行的长度
    std::vector<int> lineLengths { 0 };
    if(lengthMax > static_cast<int>(font_.size()))
    {
        int lineWidth = 0;
        for(size_t i = 0; i < texts_.size(); ++i)
        {
            lineWidth += static_cast<int>(font_.getFontGlyph(texts_[i]).advance * wordSpacing_+0.5f);//+0.5四舍五入
            if(texts_[i] == L'\n')
            {
                //RDebug() << lineWidths.back() + paddingLeft() << linepos;
                //下一行是否还有空间（预留三分之一字体大小的空白）
                if(linepos + static_cast<int>(font_.size()) + static_cast<int>(font_.size()*0.3+0.5) > lineMax) break;

                lineLengths.back() += 1;//换行符占位
                lineLengths.push_back(0);
                lineWidth = 0;//忽视换行符
                linepos += lineSize;

                continue;
            }

            if(lineWidth > lengthMax)
            {
                //RDebug() << lineWidths.back() + paddingLeft() << linepos;
                linepos += lineSize;
                //下一行是否还有空间（预留三分之一字体大小的空白）
                if(linepos + static_cast<int>(font_.size()) + static_cast<int>(font_.size()*0.3+0.5) > lineMax) break;

                lineLengths.push_back(static_cast<int>(font_.getFontGlyph(texts_[i]).advance * wordSpacing_+0.5f));
                lineWidth = static_cast<int>(font_.getFontGlyph(texts_[i]).advance * wordSpacing_+0.5f);//+0.5四舍五入

                continue;
            }

            lineLengths.back() += static_cast<int>(font_.getFontGlyph(texts_[i]).advance * wordSpacing_+0.5f);
        }
    }
    if(lineLengths.back() == 0) lineLengths.pop_back();//末尾换行

    lengthMax = width();
    lineMax = height();

    if(vAlign() == Align_Bottom) linepos = lineMax - static_cast<int>(lineLengths.size()) * lineSize - paddingBottm();
    else if(vAlign() == Align_Mind) linepos = (lineMax - static_cast<int>(lineLengths.size()) * lineSize) / 2;
    else linepos = paddingTop();//Align_Top 缺省默认

    unsigned char *data = textImage_.data();
    int wordOffset;
    RFontGlyph glyph;
    size_t textNum = 0;
    for(auto linew : lineLengths)
    {
        if(hAlign() == Align_Right) wordOffset = lengthMax - linew - paddingRight();
        else if(hAlign() == Align_Mind) wordOffset = (lengthMax - linew) / 2;
        else wordOffset = paddingLeft();//Align_Top 缺省默认
        linew += wordOffset;

        //RDebug() << "Line width:" << linew << "height:" << linepos << "offset:" << wordOffset;
        while(textNum < texts_.size())
        {
            if(texts_[textNum] == L'\n')
            {
                linepos += lineSize;
                ++textNum;
                break;
            }

            glyph = font_.getFontGlyph(texts_[textNum]);
            int startx = wordOffset + glyph.bearingX;
            int starty = linepos + static_cast<int>(font_.size()) - glyph.bearingY;
            const unsigned char* bitmap = glyph.data.get();
            unsigned char r = fontColor_.r();
            unsigned char g = fontColor_.g();
            unsigned char b = fontColor_.b();
            //RDebug() << "startx:" << startx << "starty:" << starty;
            for(int y = 0; y < glyph.height; ++y)
            {
                for(int x = 0; x < glyph.width; ++x)//, ++starty)
                {
                    if(starty+y < 0 || startx+x < 0 || startx+x > lengthMax) continue;

                    data[((starty+y)*lengthMax+startx+x)*4] = r;
                    data[((starty+y)*lengthMax+startx+x)*4+1] = g;
                    data[((starty+y)*lengthMax+startx+x)*4+2] = b;
                    data[((starty+y)*lengthMax+startx+x)*4+3] = bitmap[y*glyph.width+x];
                }
            }
            //RDebug() << "text:" << texts_[textNum] << " w:" << glyph.width << "h:" << glyph.height
                     //<< "advance:" << glyph.advance << "beaningX:" << glyph.bearingX
                     //<< "beaningY:" << glyph.bearingY << "offset:" << wordOffset;
            ++textNum;
            if(wordOffset + (glyph.advance * wordSpacing_+0.5f) >= linew) { linepos += lineSize; break; }
            else wordOffset += static_cast<int>(glyph.advance * wordSpacing_+0.5f);
        }
    }
    //若不能显示所有字符，在末尾添加一条省略线
    if(textNum < texts_.size())
    {
        linepos =  height() - (innerHeight() > 0 ? paddingBottm() + 1 : 0);
        wordOffset = paddingLeft();
        for(int i = 0; i < innerWidth(); ++i)
        {
            data[(linepos*lengthMax + i + wordOffset)*4] = 0xff;
            data[(linepos*lengthMax + i + wordOffset)*4 + 1] = 0;
            data[(linepos*lengthMax + i + wordOffset)*4 + 2] = 0;
            data[(linepos*lengthMax + i + wordOffset)*4 + 3] = 0xff;
        }
    }
    texture_.generate(textImage_);
}
