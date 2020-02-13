#include <RInfoWindow.h>
#include <iostream>

using namespace Redopera;

int main()
{
    RResource::setResourcePath("../../info-icon/");
    RInfoWIndow window;
    window.show();
    return window.exec();
}
