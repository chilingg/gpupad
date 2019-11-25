#ifndef RPOINT_H
#define RPOINT_H

class RPoint2
{
public:
    explicit RPoint2(int x = INVALID_POINT_VALUE, int y = INVALID_POINT_VALUE)
        : x_(x), y_(y) {}

    int x() const { return x_; }
    int y() const { return x_; }
    int& rx() { return x_; }
    int& ry() { return x_; }
    bool isValid() const { return x_ != INVALID_POINT_VALUE && y_ != INVALID_POINT_VALUE; }

    void setPoint(int x, int y) { x_ = x; y_ = y; }
    void setX(int x) { x_ = x; }
    void setY(int y) { y_ = y; }

protected:
    constexpr static int INVALID_POINT_VALUE = ~0u >> 1;

    int x_;
    int y_;
};

class RPoint3 : public RPoint2
{
public:
    explicit RPoint3(int x = INVALID_POINT_VALUE, int y = INVALID_POINT_VALUE, int z = INVALID_POINT_VALUE)
        : RPoint2(x, y), z_(z) {}

    int z() const { return z_; }
    int& rz() { return z_; }
    bool isValid() const {
        return x_ != INVALID_POINT_VALUE && y_ != INVALID_POINT_VALUE && z_ != INVALID_POINT_VALUE;
    }

    void setPoint(int x, int y, int z) { x_ = x; y_ = y; z_ = z; }
    void setZ(int z) { z_ = z; }

protected:
    int z_;
};

using RPoint = RPoint3;

#endif // RPOINT_H
