#include "RPoint.h"

#include "RDebug.h"

RPoint::RPoint(int x, int y):
    x_(x),
    y_(y)
{

}

int RPoint::x() const
{
    return x_;
}

int RPoint::y() const
{
    return y_;
}

int &RPoint::rx()
{
    return x_;
}

int &RPoint::ry()
{
    return y_;
}

bool RPoint::isValid() const
{
    return x_ != INVALID_POINT_VALUE && y_ != INVALID_POINT_VALUE;
}

void RPoint::setPoint(int x, int y)
{
    x_ = x;
    y_ = y;
}

void RPoint::setX(int x)
{
    x_ = x;
}

void RPoint::setY(int y)
{
    y_ = y;
}
