#ifndef RCOLOR_H
#define RCOLOR_H

#include <cstdint>

using R_RGBA = uint32_t;

class RColor
{
public:
    explicit RColor(R_RGBA color = 0xff000000u) { color_ = color; }
    RColor(unsigned r, unsigned g, unsigned b, unsigned a = 0xffu)
    {
        color_ = r & 0xffu;
        color_ |= (0xffu & g) << 8;
        color_ |= (0xffu & b) << 16;
        color_ |= a << 24;
    }

    unsigned char a() const { return color_ >> 24; }
    unsigned char b() const { return color_ >> 16 & 0xffu; }
    unsigned char g() const { return color_ >> 8 & 0xffu; }
    unsigned char r() const { return color_ & 0xffu; }

    R_RGBA rgba() const { return color_; }
    R_RGBA abgr() const { return (static_cast<R_RGBA>(r()) << 24) |
                (static_cast<R_RGBA>(g()) << 16) |
                (static_cast<R_RGBA>(b()) << 8) | a(); }

    void setA(unsigned a) { color_ = (color_ & 0xffffff) | a << 24; }
    void setB(unsigned b) { color_ = (color_ & 0xff00ffff) | (0xffu & b) << 16; }
    void setG(unsigned g) { color_ = (color_ & 0xffff00ff) | (0xffu & g) << 8; }
    void setR(unsigned r) { color_ = (color_ & 0xffffff00) | (r & 0xffu); }
    void setRGBA(R_RGBA rgba) { color_ = rgba; }

private:
    R_RGBA color_ = 0u;
};

#endif // RCOLOR_H
