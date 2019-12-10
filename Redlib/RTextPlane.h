#ifndef RTEXTPLANE_H
#define RTEXTPLANE_H

#include "RPlane.h"
#include "RResource/RFont.h"
#include <vector>

class RTextPlane : public RPlane
{
public:
    enum Typeset
    {
        HorizontalTypeset,
        VerticalTypeset
    };

    RTextPlane();
    RTextPlane(const RTextPlane &label);
    RTextPlane(const std::wstring &texts, int width, int height, const std::string &name, const RPoint &pos);
    ~RTextPlane() override;

    void setColorTexture(const RColor &color) = delete;
    void setColorTexture(R_RGBA rgba) = delete;
    void setColorTexture(unsigned r, unsigned g, unsigned b, unsigned a = 0xffu) = delete;
    void setTexture(const RImage &image) = delete;
    void setTexture(const RTexture &texture) = delete;
    void setSizeMode(SizeMode mode) = delete;
    void setFontColor(const RColor &color);
    void setFontColor(unsigned r, unsigned g, unsigned b, unsigned a = 255u);
    void setBackgroundColor(const RColor &color);
    void setBackgroundColor(unsigned r, unsigned g, unsigned b, unsigned a = 255u);

    //覆盖基类，负数转零
    void setPadding(int top, int bottom, int left, int right);
    void setPadding(int value);

    void setTexts(std::wstring texts);
    void setFontSize(unsigned size);
    bool setFont(RFont font);
    void setlineSpacing(float value);
    void setWordSpacing(float value);

    void verticalTypeset();
    void horizontalTypeset();

    void updateModelMatNow() override;

    unsigned fontSize();
    float lineSpacing();
    float wordSpacing();
    int lineHeight();

protected:

private:
    void verticalTextToTexture();
    void horizontalTextToTexture();

    std::wstring texts_;
    RColor fontColor_;
    RColor backgroundColor_;
    RImage textImage_;
    RFont font_;
    float lineSpacing_ = 1.2f;
    float wordSpacing_ = 1.0f;
    Typeset typeset_ = HorizontalTypeset;
};

#endif // RTEXTPLANE_H
