#ifndef RWHEELEVENT_H
#define RWHEELEVENT_H


class RwheelEvent
{
public:
    RwheelEvent(int x, int y, int delta);
    int x() const;
    int y() const;
    int delta() const;

private:
    const int X;
    const int Y;
    const int DELTA;
};

inline RwheelEvent::RwheelEvent(int x, int y, int delta):
    X(x),
    Y(y),
    DELTA(delta)
{

}

inline int RwheelEvent::x() const
{
    return X;
}

inline int RwheelEvent::y() const
{
    return Y;
}

inline int RwheelEvent::delta() const
{
    return DELTA;
}

#endif // RWHEELEVENT_H
