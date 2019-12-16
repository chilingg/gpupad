#include "RCursor.h"

#include "RDebug.h"

RCursor::RCursor():
    RResource("UnnameCursor")
{

}

RCursor::RCursor(const RImage &image, const std::string &name, int xhot, int yhot):
    RResource(name)
{
#ifdef R_DEBUG
    if(printError(image.channel() != 4, "Creating the cursor requires the image channel to be 4!"))
        return;
#endif

    GLFWimage img { image.width(), image.height(), const_cast<unsigned char*>(image.cdata())};
    GLFWcursor *cursor = glfwCreateCursor(&img, xhot, yhot);
    if(cursor)
        data_ = std::shared_ptr<GLFWcursor>(cursor, glfwDestroyCursor);
}

RCursor::RCursor(RCursor::CursorShape shape, const std::string &name):
    RResource(name)
{
    GLFWcursor *cursor = glfwCreateStandardCursor(shape);
    if(cursor) data_ = std::shared_ptr<GLFWcursor>(cursor, glfwDestroyCursor);
    shape_ = shape;
}

RCursor::RCursor(const RCursor &cursor):
    RResource(cursor),
    data_(cursor.data_)
{

}

RCursor::RCursor(const RCursor &&cursor):
    RResource(cursor),
    data_(cursor.data_)
{

}

RCursor &RCursor::operator=(RCursor cursor)
{
    swap(cursor);
    return *this;
}

RCursor::~RCursor()
{

}

void RCursor::swap(RCursor &cursor)
{
    RResource::swap(cursor);
    data_.swap(cursor.data_);
}

GLFWcursor *RCursor::data()
{
    return data_.get();
}

bool RCursor::isValid() const
{
    return data_ != nullptr;
}

RCursor::CursorShape RCursor::shape() const
{
    return shape_;
}

bool RCursor::creatorCursor(const RImage &image, int xhot, int yhot)
{
    GLFWimage img { image.width(), image.height(), const_cast<unsigned char*>(image.cdata())};
    GLFWcursor *cursor = glfwCreateCursor(&img, xhot, yhot);
    if(cursor)
    {
        data_.reset(cursor, glfwDestroyCursor);
        shape_ = CUSTOM_CURSOR;
    }

    return cursor == nullptr;
}

bool RCursor::creatorCursor(RCursor::CursorShape shape)
{
    GLFWcursor *cursor = glfwCreateStandardCursor(shape);
    if(cursor)
    {
        data_ = std::shared_ptr<GLFWcursor>(cursor, glfwDestroyCursor);
        shape_ = shape;
    }

    return cursor == nullptr;
}

void RCursor::freeCursor()
{
    data_.reset();
}
