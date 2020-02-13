#include <RInfoWindow.h>
#include <iostream>

using namespace Redopera;

int main()
{
    RResource::setResourcePath("../../Redopera/icon/");
    RInfoWIndow window;
    window.show();
    return window.exec();
}
