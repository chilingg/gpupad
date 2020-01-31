#include <RDebug.h>
#include <RFunction.h>

using namespace Redopera;

char print(const std::string &str)
{
    std::cout << str << ' ';
    return str[0];
}

void test(){}

int main()
{
    RFunction<char(const std::string&)> f1(print);
    RFunction<char(const std::string&)> f2([](const std::string &str){
        std::cout << str << ' ';
        return str[0];
    });
    std::function<char(const std::string&)> ff(print);
    RFunction<char(const std::string&)> f3(ff);

    rDebug << f1("func");
    rDebug << f2("lambda");
    rDebug << f3("std::func");

    return 0;
}
