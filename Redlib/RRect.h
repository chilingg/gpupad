#ifndef RRECT_H
#define RRECT_H

#include "RPoint.h"
#include "RSize.h"

class RRect
{
public:
    RRect(): RRect(RPoint2(), RSize()) {}
    RRect(int x, int y, int width, int height): x_(x), y_(y), width_(width), height_(height) {}
    RRect(const RPoint2 &bottomLeft, const RSize &size):
        x_(bottomLeft.x()), y_(bottomLeft.y()), width_(size.width()), height_(size.height()) {}

    void setSize(const RSize &size) { width_ = size.width(); height_ = size.height(); }
    void setSize(int width, int height) { width_ = width; height_ = height; }
    void setBottomLeft(const RPoint2 &pos) { x_ = pos.x(); y_ = pos.y(); }
    void setBottomLeft(int x, int y) { x_ = x; y_ = y; }
    void setTopRight(const RPoint2 &pos) { width_ = pos.x() - x_; height_ = pos.y() - y_; }
    void setTopRIght(int x, int y) { width_ = x - x_; height_ = y - y_; }

    int top() const { return y_ + height_; }
    int bottom() const { return y_; }
    int left() const { return x_; }
    int right() const { return x_ + width_; }
    RPoint2 pos() const { return RPoint2(x_, y_); }
    int width() const { return width_; }
    int height() const { return height_; }
    RSize size() const { return RSize(width_, height_); }

    bool isValid() { return width_ > 0 && height_ > 0; }
    bool isInvalid() { return width_ < 0 || height_ < 0; }
    bool isEmpty() { return width_ == 0 || height_ == 0; }

    bool contains(const RRect &rect) { return rect.x_ <= x_ && rect.y_ <= y_ && rect.width_ <= width_ && rect.height_ <= height_; }
    bool contains(const RPoint2 &pos) { return pos.x() >= x_ && pos.y() >= y_ && pos.x() <= right() && pos.y() <= top(); }
    bool contains(int x, int y) { return x >= x_ && y >= y_ && x <= right() && y <= top(); }

private:
    int x_;
    int y_;
    int width_;
    int height_;
};

#endif // RRECT_H
