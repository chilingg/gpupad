#include <RDebug.h>
#include <RArea.h>

using namespace Redopera;

int main()
{
    RArea a1;
    a1.setSize(44, -10);
    RArea a2(12, 6, -6, -3);
    a2.setMargin(2, 2, 1, 1);
    RArea a3(12, 6, RPoint(-6, -3));
    a3.setPadding(3, 3, 3, 3);
    RArea::Format format = a3.format();
    format.margin = { 0, 1, 2, 3 };
    RArea a4(format);
    format.padding = { 3, 2, 1, 0 };
    RArea::setDefaultFormat(format);
    RArea a5;

    rDebug << a1;
    rDebug << a2;
    rDebug << a3;
    rDebug << a4;
    rDebug << a5;
    rDebug << a2.rect() << a2.outerRect() << a2.innerRect();
    rDebug << a3.rect() << a3.outerRect() << a3.innerRect();
    rDebug << a4.rect() << a4.outerRect() << a4.innerRect();
    rDebug << a5.rect() << a5.outerRect() << a5.innerRect();

    return 0;
}
