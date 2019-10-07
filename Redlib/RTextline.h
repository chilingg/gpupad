#ifndef RTEXTLINE_H
#define RTEXTLINE_H

#include <RTexObject.h>
#include <RFont.h>

class RTextline : public RTexObject
{
public:
    RTextline(int width, int height);

    static RShaderProgram *textProgram;
    void render(RShaderProgram *shader) override;

    void setFontSizeRatio(float windowH, float viewProH);
    void setTexts(std::wstring texts);
    void setFontSize(int size);
    bool setFont(std::string path);

protected:
    void updataSizeMat() override;
    bool loadFontTextures();

    std::map<wchar_t, RFontTexture> textTexs;
    std::wstring texts_;
    glm::vec4 backgroundColor_;
    RFont font_;

    int fontSize_ = 32;
    float fontSizeRatio_ = 1.0;
    float rowSpacing = 1.2f;
};

inline void RTextline::setFontSizeRatio(float windowH, float viewProH)
{
    int realSize = static_cast<int>(windowH/viewProH*fontSize_);
    fontSizeRatio_ = realSize*1.0f/fontSize_;
    //RDebug() << realSize << fontSize_ << fontSizeRatio_;
    loadFontTextures();
}

inline void RTextline::setTexts(std::wstring texts)
{
    texts_ = texts;
    loadFontTextures();
}

inline void RTextline::setFontSize(int size)
{
    fontSize_ = size;
    loadFontTextures();
}

inline bool RTextline::setFont(std::string path)
{
    bool b = font_.loadFont(path);
    loadFontTextures();
    return b;
}

#endif // RTEXTLINE_H
