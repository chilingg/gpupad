#ifndef RRESIZEEVENT_H
#define RRESIZEEVENT_H

class RResizeEvent
{
public:
    enum Pattern{
        Keep,
        Ignore
    };
    RResizeEvent(int width, int height, Pattern pattern = Keep);
    int width() const;
    int height() const;
    void setPattern(Pattern pattern);
    Pattern pattern() const;

private:
    const int WIDTH;
    const int HEIGHT;
    Pattern p;
};

#endif // RRESIZEEVENT_H

inline RResizeEvent::RResizeEvent(int width, int height, Pattern pattern):
    WIDTH(width),
    HEIGHT(height),
    p(pattern)
{

}

inline int RResizeEvent::width() const
{
    return WIDTH;
}

inline int RResizeEvent::height() const
{
    return HEIGHT;
}

inline void RResizeEvent::setPattern(RResizeEvent::Pattern pattern)
{
    p = pattern;
}

inline RResizeEvent::Pattern RResizeEvent::pattern() const
{
    return p;
}
