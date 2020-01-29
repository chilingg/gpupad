#include <RDebug.h>
#include <RSigslot.h>

using namespace Redopera;

class t1
{
public:
    void print(char c)
    {
        rDebug << "Ksy: " << c;
    }

    _RSLOT_TAIL_
};

class t2
{
public:
    RSignal<char> entered;
};

t2 tow;
t1 one;

int main()
{
    t1 three;
    tow.entered.connect(&one, &t1::print);
    tow.entered.connect(&three, &t1::print);
    std::thread th([]{
        char c;
        do {
            std::cin >> c;
            tow.entered.emit(c);
            if(c == 'n')
            {
                tow.entered.disconnect(&one);
            }
        } while(c != 'q');
    });
    th.join();

    return 0;
}
