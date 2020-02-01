#ifndef RCORSOR_H
#define RCORSOR_H

#include "RResource.h"
#include "ROpenGL.h"
#include "RImage.h"

namespace Redopera {

class RCursor : public RResource
{
    friend void swap(RCursor &cs1, RCursor &cs2);

public:
    enum class CursorShape
    {
        ARROW_CURSOR        = GLFW_ARROW_CURSOR,
        IBEAM_CURSOR        = GLFW_IBEAM_CURSOR,
        CROSSHAIR_CURSOR    = GLFW_CROSSHAIR_CURSOR,
        HAND_CURSOR         = GLFW_HAND_CURSOR,
        HRESIZE_CURSOR      = GLFW_HRESIZE_CURSOR,
        VRESIZE_CURSOR      = GLFW_VRESIZE_CURSOR,
        CUSTOM_CURSOR
    };

    RCursor();
    RCursor(const RImage &image, const std::string &name = "Cursor", int xhot = 0, int yhot = 0);
    RCursor(CursorShape shape, const std::string &name = "Cursor");
    RCursor(const RCursor &cursor);
    RCursor(const RCursor &&cursor);
    RCursor& operator=(RCursor cursor);
    ~RCursor() = default;
    void swap(RCursor &cursor);

    bool isValid() const;
    CursorShape shape() const;
    GLFWcursor* data() const;

    bool load(const RImage &image, int xhot = 0, int yhot = 0);
    bool load(CursorShape shape);
    void release();

private:
    std::shared_ptr<GLFWcursor> data_;
    CursorShape shape_ = CursorShape::CUSTOM_CURSOR;
};

} // Redopera

void swap(Redopera::RCursor &cursor1, Redopera::RCursor &cursor2);

#endif // RCORSOR_H
