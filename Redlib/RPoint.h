#ifndef RPOINT_H
#define RPOINT_H


class RPoint
{
    constexpr static int INVALID_POINT_VALUE = ~0;
public:
    explicit RPoint(int x = INVALID_POINT_VALUE, int y = INVALID_POINT_VALUE);

    int x() const;
    int y() const;
    int& rx();
    int& ry();
    bool isValid() const;

    void setPoint(int x, int y);
    void setX(int x);
    void setY(int y);

private:
    int x_;
    int y_;
};

#endif // RPOINT_H
