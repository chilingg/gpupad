#ifndef RKEYEVENT_H
#define RKEYEVENT_H

class RKeyEvent
{
public:
    RKeyEvent(int key, int modifiers);
    int key() const;
    int modifiers() const;

private:
    const int KEY;
    const int MODIFIERS;
};

inline RKeyEvent::RKeyEvent(int key, int modifiers):
    KEY(key),
    MODIFIERS(modifiers)
{

}

inline int RKeyEvent::key() const
{
    return KEY;
}

inline int RKeyEvent::modifiers() const
{
    return MODIFIERS;
}

#endif // RKEYEVENT_H
