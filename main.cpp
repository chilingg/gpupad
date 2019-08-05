#include <rmaincontrol.h>

int main()
{
    RMainControl controler;
    controler.initialize();
    controler.exec();

    return 0;
}
