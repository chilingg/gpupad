#ifndef RCURSOR_H
#define RCURSOR_H

#include "RResource.h"
#include "ROpenGL.h"
#include "RImage.h"

#include <memory>

class RCursor : public RResource
{
    friend void swap(RCursor &cursor1, RCursor &cursor2);
public:
    enum CursorShape {
        ARROW_CURSOR   = GLFW_ARROW_CURSOR,
        IBEAM_CURSOR   = GLFW_IBEAM_CURSOR,
        CROSSHAIR_CURSOR   = GLFW_CROSSHAIR_CURSOR,
        HAND_CURSOR   = GLFW_HAND_CURSOR,
        HRESIZE_CURSOR   = GLFW_HRESIZE_CURSOR,
        VRESIZE_CURSOR   = GLFW_VRESIZE_CURSOR,
        CUSTOM_CURSOR
    };

    RCursor();
    RCursor(const RImage &image, const std::string &name, int xhot = 0, int yhot = 0);
    RCursor(CursorShape shape, const std::string &name);
    RCursor(const RCursor &cursor);
    RCursor(const RCursor &&cursor);
    RCursor& operator=(RCursor cursor);
    ~RCursor();
    void swap(RCursor &cursor);

    bool isValid() const;
    CursorShape shape() const;
    GLFWcursor* data();
    bool creatorCursor(const RImage &image, int xhot = 0, int yhot = 0);
    bool creatorCursor(CursorShape shape);
    void freeCursor();

private:
    std::shared_ptr<GLFWcursor> data_;
    CursorShape shape_ = CUSTOM_CURSOR;
};

void swap(RCursor &cursor1, RCursor &cursor2);

#endif // RCURSOR_H
