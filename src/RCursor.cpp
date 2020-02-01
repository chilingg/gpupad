#include "resource/RCursor.h"

Redopera::RCursor::RCursor():
    RResource("Cursor", typeid(this).name())
{

}

Redopera::RCursor::RCursor(const Redopera::RImage &image, const std::string &name, int xhot, int yhot):
    RResource(name, typeid(this).name())
{
    load(image, xhot, yhot);
}

Redopera::RCursor::RCursor(Redopera::RCursor::CursorShape shape, const std::string &name):
    RResource(name, typeid(this).name())
{
    load(shape);
}

Redopera::RCursor::RCursor(const Redopera::RCursor &cursor):
    RResource(cursor),
    data_(cursor.data_),
    shape_(cursor.shape_)
{

}

Redopera::RCursor::RCursor(const Redopera::RCursor &&cursor):
    RResource(std::move(cursor)),
    data_(std::move(cursor.data_)),
    shape_(cursor.shape_)
{

}

Redopera::RCursor &Redopera::RCursor::operator=(Redopera::RCursor cursor)
{
    swap(cursor);
    return *this;
}

void Redopera::RCursor::swap(Redopera::RCursor &cursor)
{
    RResource::swap(cursor);
    data_.swap(cursor.data_);
    using std::swap;
    swap(shape_, cursor.shape_);
}

bool Redopera::RCursor::isValid() const
{
    return data_ != nullptr;
}

Redopera::RCursor::CursorShape Redopera::RCursor::shape() const
{
    return shape_;
}

GLFWcursor *Redopera::RCursor::data() const
{
    return data_.get();
}

bool Redopera::RCursor::load(const Redopera::RImage &image, int xhot, int yhot)
{
    GLFWimage img { image.width(), image.height(), image.data() };
    GLFWcursor *cursor = glfwCreateCursor(&img, xhot, yhot);
    if(cursor)
    {
        data_.reset(cursor, glfwDestroyCursor);
        return false;
    }

    return true;
}

bool Redopera::RCursor::load(Redopera::RCursor::CursorShape shape)
{
    GLFWcursor *cursor = glfwCreateStandardCursor(static_cast<int>(shape));
    if(cursor)
    {
        data_.reset(cursor, glfwDestroyCursor);
        return false;
    }

    shape_ = shape;
    return true;
}

void Redopera::RCursor::release()
{
    data_.reset();
}

void swap(Redopera::RCursor &cursor1, Redopera::RCursor &cursor2)
{
    cursor1.swap(cursor2)
}
