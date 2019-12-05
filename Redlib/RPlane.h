#ifndef RPLANE_H
#define RPLANE_H

#include "RResource/RShaderProgram.h"
#include "RResource/RTexture.h"
#include "RPoint.h"
#include "RColor.h"
#include "RSize.h"
#include "RMaths.h"

#define offsetBuffer(offset) ( reinterpret_cast<void*>(offset) )

class RPlane
{
public:
    enum SizeMode {
        Fixed,
        Auto,
        Cover,
        Contain
    };
    enum Alignment {
        Align_Left,
        Align_Right,
        Align_Mind,
        Align_Top,
        Align_Bottom
    };

    static void setPlaneDefaultViewpro(int left, int right, int buttom, int top, int near= -1, int far = 1);
    static void setPlaneDefaultCameraPos(int x, int y, int z = 0);

    explicit RPlane(RShaderProgram *program = nullptr, const std::string &name = "Plane");
    RPlane(const RPlane &plane);
    RPlane(int width, int height, const std::string &name, RPoint pos, RShaderProgram *program = nullptr);
    virtual ~RPlane();

    void setSize(int width, int height);
    void setSize(RSize size);

    void setPosition(int x, int y, int z = 0);
    void setPositionX(int value);
    void setPositionY(int value);
    void setPositionZ(int value);

    void setMargin(int top, int bottom, int left, int right);
    void setMargin(int value);
    void setPadding(int top, int bottom, int left, int right);
    void setPadding(int value);

    void setColorTexture(RColor color);
    void setColorTexture(R_RGBA rgba);
    void setColorTexture(unsigned r, unsigned g, unsigned b, unsigned a = 0xffu);
    void setTexture(const RImage &image);
    void setTexture(const RTexture &texture);
    void setSizeMode(SizeMode mode);
    void setAlignment(Alignment hAlign, Alignment vAlign);
    void setShaderProgram(const RShaderProgram &program, RUniformLocation modelLoc);
    void rename(std::string name);

    void rotateX(float x);
    void rotateY(float y);
    void rotateZ(float z);

    void flipH();
    void flipV();

    int width() const { return width_; }
    int height() const { return height_; }
    int innerWidth() const { return width_ - paddingLeft_ - paddingRight_; }
    int innerHeight() const { return height_ - paddingTop_ - paddingBottom_; }
    int outerWidth() const { return width_ + marginLeft_ + marginRight_; }
    int outerHeight() const { return height_ + marginTop_ + marginBottom_; }

    int paddingLeft() const { return paddingLeft_; }
    int paddingRight() const { return paddingRight_; }
    int paddingTop() const { return paddingTop_; }
    int paddingBottm() const { return paddingBottom_; }

    RPoint pos() const { return pos_; }
    int x() const { return pos_.x(); }
    int& rx() { return pos_.rx(); }
    int y() const { return pos_.y(); }
    int& ry() { return pos_.ry(); }

    bool isFlipV() const { return flipV_; }
    bool isFlipH() const { return flipH_; }

    SizeMode sizeMode() const { return sizeMode_; }
    Alignment vAlign() const { return vAlign_; }
    Alignment hAlign() const { return hAlign_; }
    const std::string& name() const { return name_; }
    const RMatrix4& modelMat() const { return modelMat_; }

    void render();
    void render(RMatrix4 modelMat);
    void renderUseSizeModel(RMatrix4 modelMat);
    void renderUsePositionAndSizeModel(RMatrix4 modelMat);
    virtual void updateModelMatNow();
    void updateModelMat();
#ifdef R_DEBUG
    //渲染边距线框
    void RenderLineBox(const RMatrix4 &projection, const RMatrix4 &view);
    void RenderLineBox(int left, int right, int buttom, int top, RPoint pos = RPoint(0, 0));
#endif

protected:
    virtual void renderControl();
    void updateModelMatOver();

    RMatrix4 rotateMat_;
    RMatrix4 modelMat_;
    RShaderProgram shaders_;
    RTexture texture_;
    RUniformLocation modelLoc_;

private:
    static RShaderProgram lineBoxsProgram;
    static GLuint lineBoxVAO;
    static RShaderProgram planeSProgram;
    static GLuint planeVAO, planeVBO;
    static unsigned long count;

    std::string name_;
    int width_;
    int height_;
    RPoint pos_;

    bool dirty_ = true;
    bool flipH_ = false;
    bool flipV_ = false;
    //外边距
    int marginTop_ = 0;
    int marginBottom_ = 0;
    int marginLeft_ = 0;
    int marginRight_ = 0;
    //内边距
    int paddingTop_ = 0;
    int paddingBottom_ = 0;
    int paddingLeft_ = 0;
    int paddingRight_ = 0;
    SizeMode sizeMode_ = Auto;//RTextLabel依赖Auto初始值
    Alignment vAlign_ = Align_Mind;
    Alignment hAlign_ = Align_Mind;
};

#endif // RPLANE_H
