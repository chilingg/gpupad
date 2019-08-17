#include <RWindow.h>

int main()
{
    RWindow controler;

    if(!controler.initialize())
        return -1;
    controler.exec();

    return 0;
}
