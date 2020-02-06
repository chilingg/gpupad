#include <RDebug.h>
#include <RContext.h>
#include <rsc/RPack.h>

using namespace Redopera;

int main()
{
    std::string path1 = "Makefile";
    RPack pack;
    RPack pack3("packtest");
    pack.packing(path1);
    rDebug << pack.empty() << pack.size();
    rDebug << pack3.empty() << pack3.size();
    rDebug << pack.equivalent(pack3);
    pack.save("packtest");

    return 0;
}
