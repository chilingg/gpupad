#ifndef RRECT_H
#define RRECT_H

#include "RPoint.h"
#include "RSize.h"

namespace Redopera {

class RRect
{
public:
    RRect(): RRect(RPoint2(0), RSize()) {}
    RRect(int x, int y, int width, int height): x_(x), y_(y), width_(width), height_(height) {}
    RRect(const RPoint2 &bottomLeft, const RSize &size):
        x_(bottomLeft.x()), y_(bottomLeft.y()), width_(size.width()), height_(size.height()) {}
    RRect(const RPoint2 &bottomLeft, const RPoint2 &topRight):
        x_(bottomLeft.x()), y_(bottomLeft.y()),
        width_(topRight.x() - bottomLeft.x()),
        height_(topRight.y() - bottomLeft.y()) {}

    void setSize(const RSize &size) { width_ = size.width(); height_ = size.height(); }
    void setSize(int width, int height) { width_ = width; height_ = height; }

    void setBottomLeft(const RPoint2 &pos) { x_ = pos.x(); y_ = pos.y(); }
    void setBottomLeft(int x, int y) { x_ = x; y_ = y; }
    void setTopRight(const RPoint2 &pos) { x_ = pos.x() - width_; y_ = pos.y() - height_; }
    void setTopRIght(int x, int y) { x_ = x - width_; y_ = y - height_; }
    void setCenter(const RPoint2 &pos) { x_ = pos.x() - width_/2; y_ = pos.y() - height_/2; }
    void setCenter(int x, int y) { x_ = x - width_/2; y_ = y - height_/2; }

    int top() const { return y_ + height_; }
    int bottom() const { return y_; }
    int left() const { return x_; }
    int right() const { return x_ + width_; }
    RPoint2 bottomLeft() const { return RPoint2(x_, y_); }
    int width() const { return width_; }
    int height() const { return height_; }
    RSize size() const { return RSize(width_, height_); }

    bool isValid() { return width_ > 0 && height_ > 0; }
    bool isInvalid() { return width_ < 0 || height_ < 0; }
    bool isEmpty() { return width_ == 0 || height_ == 0; }

    bool contains(const RRect &rect)
    {
        return rect.x_ <= x_ && rect.y_ <= y_ && rect.width_ <= width_ && rect.height_ <= height_;
    }
    bool contains(const RPoint2 &pos)
    {
        return pos.x() >= x_ && pos.y() >= y_ && pos.x() <= right() && pos.y() <= top();
    }
    bool contains(int x, int y) { return x >= x_ && y >= y_ && x <= right() && y <= top(); }

    RRect MirrorH(int axis = 0)
    {
        int x = (axis-x_)*2 + x_ + -width_;
        return RRect(x, y_, width_, height_);
    }
    RRect MirrorV(int axis = 0)
    {
        int y = (axis-y_)*2 + y_ + -height_;
        return RRect(x_, y, width_, height_);
    }

private:
    int x_;
    int y_;
    int width_;
    int height_;
};

} // Redopera

#endif // RRECT_H
