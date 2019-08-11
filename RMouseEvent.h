#ifndef RMOUSEEVENT_H
#define RMOUSEEVENT_H

class RMouseEvent
{
public:
    RMouseEvent(int x, int y, int button);
    int x() const;
    int y() const;
    int button() const;

private:
    const int X;
    const int Y;
    const int BUTTON;
};

inline RMouseEvent::RMouseEvent(int x, int y, int button):
    X(x),
    Y(y),
    BUTTON(button)
{

}

inline int RMouseEvent::x() const
{
    return X;
}

inline int RMouseEvent::y() const
{
    return Y;
}

inline int RMouseEvent::button() const
{
    return BUTTON;
}

#endif // RMOUSEEVENT_H
