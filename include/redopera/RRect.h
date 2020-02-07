#ifndef RRECT_H
#define RRECT_H

#include "RPoint.h"
#include "RSize.h"

namespace Redopera {

class RRect
{
public:
    RRect();
    RRect(int width, int height, int x = 0, int y = 0);
    RRect(int width, int height, const RPoint2 &bottomLeft);
    RRect(const RSize &size, const RPoint2 &bottomLeft);
    RRect(const RPoint2 &bottomLeft, const RPoint2 &topRight);

    void setSize(const RSize &size);
    void setSize(int width, int height);

    void setBottomLeft(const RPoint2 &pos);
    void setBottomLeft(int x, int y);
    void setTopRight(const RPoint2 &pos);
    void setTopRIght(int x, int y);
    void setCenter(const RPoint2 &pos);
    void setCenter(int x, int y);
    void set(int width, int height, int x, int y);
    void set(const RSize &size, const RPoint2 &bottomLeft);

    int top() const;
    int bottom() const;
    int left() const;
    int right() const;
    RPoint2 bottomLeft() const;
    RPoint2 bottomRight() const;
    RPoint2 topLeft() const;
    RPoint2 topRIght() const;
    RPoint2 center() const;
    int width() const;
    int height() const;
    RSize size() const;

    bool isValid();
    bool isInvalid();
    bool isEmpty();

    bool contains(const RRect &rect);
    bool contains(const RPoint2 &pos);
    bool contains(int x, int y);

    RRect MirrorH(int axis = 0);
    RRect MirrorV(int axis = 0);

private:
    int x_;
    int y_;
    int width_;
    int height_;
};

inline RRect::RRect():
    RRect(RSize(), RPoint2(0))
{}

inline RRect::RRect(int width, int height, int x, int y):
    x_(x), y_(y), width_(width), height_(height)
{}

inline RRect::RRect(int width, int height, const RPoint2 &bottomLeft):
    x_(bottomLeft.x()), y_(bottomLeft.y()), width_(width), height_(height)
{

}

inline RRect::RRect(const RSize &size, const RPoint2 &bottomLeft):
    x_(bottomLeft.x()), y_(bottomLeft.y()), width_(size.width()), height_(size.height())
{}

inline RRect::RRect(const RPoint2 &bottomLeft, const RPoint2 &topRight):
    x_(bottomLeft.x()), y_(bottomLeft.y()),
    width_(topRight.x() - bottomLeft.x()),
    height_(topRight.y() - bottomLeft.y())
{}

inline void RRect::setSize(const RSize &size)
{
    width_ = size.width(); height_ = size.height();
}

inline void RRect::setSize(int width, int height)
{
    width_ = width; height_ = height;
}

inline void RRect::setBottomLeft(const RPoint2 &pos)
{
    x_ = pos.x(); y_ = pos.y();
}

inline void RRect::setBottomLeft(int x, int y)
{
    x_ = x; y_ = y;
}

inline void RRect::setTopRight(const RPoint2 &pos)
{
    x_ = pos.x() - width_; y_ = pos.y() - height_;
}

inline void RRect::setTopRIght(int x, int y)
{
    x_ = x - width_; y_ = y - height_;
}

inline void RRect::setCenter(const RPoint2 &pos)
{
    x_ = pos.x() - width_/2; y_ = pos.y() - height_/2;
}

inline void RRect::setCenter(int x, int y)
{
    x_ = x - width_/2; y_ = y - height_/2;
}

inline void RRect::set(int width, int height, int x, int y)
{
    width_ = width; height_ = height;
    x_ = x; y_ = y;
}

inline void RRect::set(const RSize &size, const RPoint2 &bottomLeft)
{
    width_ = size.width(); height_ = size.height();
    x_ = bottomLeft.x(); y_ = bottomLeft.y();
}

inline int RRect::top() const
{
    return y_ + height_;
}

inline int RRect::bottom() const
{
    return y_;
}

inline int RRect::left() const
{
    return x_;
}

inline int RRect::right() const
{
    return x_ + width_;
}

inline RPoint2 RRect::bottomLeft() const
{
    return RPoint2(x_, y_);
}

inline RPoint2 RRect::bottomRight() const
{
    return RPoint2(x_ + width_, y_);
}

inline RPoint2 RRect::topLeft() const
{
    return RPoint2(x_, y_ + height_);
}

inline RPoint2 RRect::topRIght() const
{
    return RPoint2(x_ + width_, y_ + height_);
}

inline RPoint2 RRect::center() const
{
    return RPoint2(x_ + width_/2, y_ + height_/2);
}

inline int RRect::width() const
{
    return width_;
}

inline int RRect::height() const
{
    return height_;
}

inline RSize RRect::size() const
{
    return RSize(width_, height_);
}

inline bool RRect::isValid()
{
    return width_ > 0 && height_ > 0;
}

inline bool RRect::isInvalid()
{
    return width_ < 0 || height_ < 0;
}

inline bool RRect::isEmpty()
{
    return width_ == 0 || height_ == 0;
}

inline bool RRect::contains(const RRect &rect)
{
    return rect.x_ <= x_ && rect.y_ <= y_ && rect.width_ <= width_ && rect.height_ <= height_;
}

inline bool RRect::contains(const RPoint2 &pos)
{
    return pos.x() >= x_ && pos.y() >= y_ && pos.x() <= right() && pos.y() <= top();
}

inline bool RRect::contains(int x, int y)
{
    return x >= x_ && y >= y_ && x <= right() && y <= top();
}

inline RRect RRect::MirrorH(int axis)
{
    int x = (axis-x_)*2 + x_ + -width_;
    return RRect(x, y_, width_, height_);
}

inline RRect RRect::MirrorV(int axis)
{
    int y = (axis-y_)*2 + y_ + -height_;
    return RRect(x_, y, width_, height_);
}

} // Redopera

#endif // RRECT_H
