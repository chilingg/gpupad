#ifndef RAREA_H
#define RAREA_H

#include "RPoint.h"
#include "RSize.h"
#include "RRect.h"

namespace Redopera {

class RArea
{
public:
    enum class Mode
    {
        Fixed,
        Auto,
        Cover,
        Contain
    };

    enum class Align
    {
        Left,
        Right,
        Mind,
        Top,
        Bottom
    };

    struct Format
    {
        RSize size;
        RPoint pos;
        struct { int t, b, l, r;
               } margin { 0, 0, 0, 0 }, padding { 0, 0, 0, 0 };
        struct { Align v, h;
               } align { Align::Left, Align::Bottom};
        Mode mode = Mode::Auto;
        struct { float x, y, z;
               } rotate { 0.0f, 0.0f, 0.0f };
        bool flipH = false;
        bool flipV = false;
        bool dirty = true;
        int minW = 0;
        int minH = 0;
        int maxW = ~0u >> 1;
        int maxH = ~0u >> 1;
    };

    static void setDefaultFormat(Format format);
    static Format getDefaultFormat();

    RArea();
    RArea(int width, int height, int x, int y, int z = 0);
    RArea(int width, int height, const RPoint &pos);
    RArea(const RSize &size, const RPoint &pos);
    RArea(const RRect &rect, int z = 0);
    RArea(const Format &format);
    RArea(RArea &area);
    ~RArea() = default;

    void setMinSize(int minw, int minh);
    void setMinSize(const RSize &size);
    void setMaxSize(int maxw, int maxh);
    void setMaxSize(const RSize &size);

    void setSize(int width, int height);
    void setSize(const RSize &size);
    void setWidth(int width);
    void setHeight(int height);

    void setPos(int x, int y, int z = 0);
    void setPos(const RPoint &pos);
    void setPosX(int x);
    void setPosY(int y);
    void setPosZ(int z);

    void setOuterPos(const RPoint &pos);
    void setCenterPos(const RPoint &pos);
    void setInnerPos(const RPoint &pos);

    void setMargin(int top, int bottom, int left, int right);
    void setMargin(int value);
    void setPadding(int top, int bottom, int left, int right);
    void setPadding(int value);

    void setMode(Mode mode);
    void setAlign(Align v, Align h);

    void setDirty();
    void clearDirty();

    void rotateX(float x);
    void rotateY(float y);
    void rotateZ(float z);

    void flipH();
    void flipV();

    RRect rect() const;
    RSize size() const;
    RPoint pos() const;
    int width() const;
    int height() const;
    int x() const;
    int y() const;
    int z() const;
    int& rx();
    int& ry();
    int& rz();

    RPoint centerPos() const;

    RRect outerRect() const;
    RSize outerSize() const;
    RPoint outerPos() const;
    int outerWidth() const;
    int outerHeight() const;

    RRect innerRect() const;
    RSize innerSize() const;
    RPoint innerPos() const;
    int innerWidth() const;
    int innerHeight() const;

    Mode mode() const;
    Align vAlign() const;
    Align hAlign() const;

    RSize maxSize() const;
    RSize minSize() const;

    bool isFlipV() const;
    bool isFlipH() const;
    bool isDirty() const;

    const Format &format() const;

private:
    static Format areaFormat;
    Format format_;
};

} // Redopera

#endif // RAREA_H
