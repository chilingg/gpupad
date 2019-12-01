#ifndef RTEXTPLANE_H
#define RTEXTPLANE_H

#include "RPlane.h"
#include "RResource/RFont.h"

class RTextPlane : public RPlane
{
public:
    RTextPlane(RShaderProgram *program = nullptr);
    RTextPlane(const RTextPlane &label);
    RTextPlane(std::wstring texts, int width, int height, const std::string &name, RPoint pos, RShaderProgram *program = nullptr);
    ~RTextPlane() override;

    void setColorTexture(RColor color) = delete;
    void setColorTexture(R_RGBA rgba) = delete;
    void setColorTexture(unsigned r, unsigned g, unsigned b, unsigned a = 0xffu) = delete;
    void setTexture(const RImage &image) = delete;
    void setTexture(const RTexture &texture) = delete;
    void setSizeMode(SizeMode mode) = delete;

    //覆盖，负数转零
    void setPadding(int top, int bottom, int left, int right);
    void setPadding(int value);

    void setTexts(std::wstring texts);
    void setFontSize(unsigned size);
    bool setFont(RFont font);
    void setlineSpacing(float value);
    void setWordSpacing(float value);

protected:
    void updateModelMatNow() override;

private:
    std::wstring texts_;
    RImage textImage_;
    RFont font_;
    float lineSpacing_ = 1.2f;
    float wordSpacing_ = 1.0f;
};

#endif // RTEXTPLANE_H
