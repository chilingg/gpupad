#include <RWindow.h>

using namespace Redopera;

int main()
{
    RWindow::WindowFormat format;
    format.debug = true;
    format.versionMajor = 4;
    format.versionMinor = 3;

    RWindow window(format);
    window.show();

    return window.exec();
}
