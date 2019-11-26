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
        FIxed,
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

    static void setViewpro(int x, int y, int width, int height, int near = -1, int far = 1);
    static void setViewproMove(int x, int y, int z = 0);

    RPlane();
    RPlane(const RPlane &plane);
    RPlane(const std::string &name, int width, int height, RPoint pos = RPoint(0, 0, 0));
    ~RPlane();

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
    RPoint pos() const { return pos_; }
    int x() const { return pos_.x(); }
    int& rx() { return pos_.rx(); }
    int y() const { return pos_.y(); }
    int& ry() { return pos_.ry(); }
    bool isFlipV() const { return flipV_; }
    bool isFlipH() const { return flipH_; }

    void render();
#ifdef R_DEBUG
    //渲染边距线框
    void displayLineBox(const RMatrix4 &projection, const RMatrix4 &view);
    void displayLineBox(int left, int right, int buttom, int top, RPoint pos = RPoint(0, 0));
#endif

private:
    static RShaderProgram lineBoxsProgram;
    static GLuint lineBoxVAO;
    static RShaderProgram planeSProgram;
    static GLuint planeVAO, planeVBO;
    static RTexture whiteTex;
    static UniformLocation modelLoc;
    static UniformLocation viewLoc;
    static UniformLocation projectionLoc;
    static int count;

    void updataModelMat();
    void updataModelMatNow();

    std::string name_;
    int width_;
    int height_;
    RPoint pos_;
    RMatrix4 rotateMat_;
    RMatrix4 modelMat_;
    RTexture texture_;
    RShaderProgram shaders_;

    bool dirty_ = false;
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
    SizeMode sizeMode_ = Auto;
    Alignment vAlign_ = Align_Bottom;
    Alignment hAlign_ = Align_Left;
};

#endif // RPLANE_H
