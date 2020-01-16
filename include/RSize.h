#ifndef RSIZE_H
#define RSIZE_H

namespace Redopera {

class RSize
{
public:
    RSize(): RSize(0, 0) {}
    RSize(int width, int height): width_(width), height_(height) {}

    int width() const { return width_; }
    int height() const { return height_; }

    bool isEmpty() const { return (width_ == 0) && (height_ == 0); }
    bool isInvalid() const { return (width_ <= 0) || (height_ <= 0); }
    bool isValid() const { return (width_ > 0) && (height_ > 0); }

private:
    int width_;
    int height_;
};

} // Redopera

#endif // RSIZE_H
