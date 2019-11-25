#ifndef RCOLOR_H
#define RCOLOR_H

#include <cstdint>

using R_RGB = uint32_t;

class RColor
{
public:
    explicit RColor(R_RGB color = 0xffu) { color_ = color; }
    RColor(unsigned r, unsigned g, unsigned b, unsigned a = 0xffu) {
        color_ = a & 0xffu;
        color_ |= (0xffu & b) << 8;
        color_ |= (0xffu & g) << 16;
        color_ |= r << 24;
    }

    unsigned r() const { return color_ >> 24 & 0xffu; }
    unsigned g() const { return color_ >> 16 & 0xffu; }
    unsigned b() const { return color_ >> 8 & 0xffu; }
    unsigned a() const { return color_ & 0xffu; }

    void setR(unsigned r) { color_ = (color_ & 0xffffff) | r << 24; }
    void setG(unsigned g) { color_ = (color_ & 0xff00ffff) | (0xffu & g) << 16; }
    void setB(unsigned b) { color_ = (color_ & 0xffff00ff) | (0xffu & b) << 8; }
    void setA(unsigned a) { color_ = (color_ & 0xffffff00) | (a & 0xffu); }
    void setRGB(R_RGB rgb) { color_ = rgb; }

private:
    R_RGB color_ = 0u;
};

#endif // RCOLOR_H
