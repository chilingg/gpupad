#include <RDebug.h>
#include <resource/RLuaScript.h>
#include <fstream>

using namespace Redopera;

constexpr char func1[] = "function func(arg1, arg2) return arg2 .. arg1 end";
constexpr char func2[] = "function func(arg1) return arg1 .. 2222 end";

int main()
{
    RLuaScript scp(func2);

    if(scp.call("func", {}, {"hello"}))
        rDebug << scp.getString();

    RData *data;
    std::ifstream file;
    size_t size;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open("/xxxx/test.lua", std::ios::binary | std::ios::ate);
        size = file.tellg();
        data = new RData[size];
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char*>(data), size);
        file.close();
    }
    catch(...)
    {
        prError("Failed");
        return 1;
    }

    scp.import(data, size, "tt");
    if(scp.call("fun", {}, {"hello"}))
        rDebug << scp.getString();

    if(scp.call("func", {}, {"hello"}))
        rDebug << scp.getString();

    return 0;
}
