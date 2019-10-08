//字体纹理映射窗口像素时，若坐标值刚好为x.5，则一个纹理像素映射两个屏幕像素，产生模糊现象

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

    void setTexts(std::wstring texts);
    void setFontSize(int size);
    bool setFont(std::string path);
    void setRowSpacing(float value);

protected:
    void updataSizeMat() override;

    bool loadFontTextures();

    std::vector<int> fontPosOffset;
    std::map<wchar_t, RFontTexture> textTexs;
    std::wstring texts_;
    glm::vec4 backgroundColor_;
    RFont font_;

    int fontSize_ = 32;
    //float fontSizeRatio_ = 1.0;//废弃
    float rowSpacing = 1.2f;
};

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

inline void RTextline::setRowSpacing(float value)
{
    rowSpacing = value;
}

#endif // RTEXTLINE_H
