#ifndef RWHEELEVENT_H
#define RWHEELEVENT_H

class RWheelEvent
{
public:
    RWheelEvent(int x, int y, int delta);
    int x() const;
    int y() const;
    int delta() const;

private:
    const int X;
    const int Y;
    const int DELTA;
};

inline RWheelEvent::RWheelEvent(int x, int y, int delta):
    X(x),
    Y(y),
    DELTA(delta)
{

}

inline int RWheelEvent::x() const
{
    return X;
}

inline int RWheelEvent::y() const
{
    return Y;
}

inline int RWheelEvent::delta() const
{
    return DELTA;
}

#endif // RWHEELEVENT_H
