#ifndef RSIZE_H
#define RSIZE_H

class RSize
{
public:
    RSize();
    RSize(int width, int height);

    int width() const;
    int height() const;

    bool isNull() const;
    bool isEmpty() const;
    bool isValid() const;

private:
    int _width;
    int _height;
};

inline int RSize::width() const
{
    return _width;
}

inline int RSize::height() const
{
    return _height;
}

inline bool RSize::isNull() const
{
    return _width == 0 && _height == 0;
}

inline bool RSize::isEmpty() const
{
    return _width <= 0 || _height <= 0;
}

inline bool RSize::isValid() const
{
    return _width > 0 && _height > 0;
}

#endif // RSIZE_H
