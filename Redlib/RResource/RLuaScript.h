#ifndef RLUASCRIPT_H
#define RLUASCRIPT_H

extern "C" {
#include "Lua/lua.h"
#include "Lua/lualib.h"
#include "Lua/lauxlib.h"
}
#include "RResource/RResource.h"

#include <memory>
#include <initializer_list>

enum class RLuaType
{
    None = LUA_TNONE,
    Nin = LUA_TNIL,
    Bool = LUA_TBOOLEAN,
    LightUserData = LUA_TLIGHTUSERDATA,
    Number = LUA_TNUMBER,
    String = LUA_TSTRING,
    Table = LUA_TTABLE,
    Function = LUA_TFUNCTION,
    UserData = LUA_TUSERDATA,
    Thread = LUA_TTHREAD
};

class RLuaScript : public RResource
{
    friend void swap(RLuaScript &rls1, RLuaScript &rls2);

public:
    RLuaScript();
    RLuaScript(const std::string &path, const std::string &name);
    RLuaScript(const unsigned char *buff, size_t size, const std::string &name);
    RLuaScript(const RLuaScript &rls);
    RLuaScript(const RLuaScript &&rls);
    RLuaScript& operator=(RLuaScript rls);
    ~RLuaScript();
    void swap(RLuaScript &rls);

    bool isValid() const;
    bool isValidIndex(int index) const;

    bool valueIsBool(int index = -1) const;
    bool valueIsFunction(int index = -1) const;
    bool valueIsNumber(int index = -1) const;
    bool valueIsString(int index = -1) const;
    bool valueIsTable(int index = -1) const;
    RLuaType valueType(int index) const;
    std::string typeName(RLuaType type);

    int stackSize() const;

    bool getBool(int index = -1);
    double getNumber(int index = -1);
    long long getInteger(int index = -1);
    std::string getString(int index = -1);
    long long getTableValueToInt(const std::string key, int index = -1);
    double getTableValueToDouble(const std::string key, int index = -1);
    std::string getTableValueToString(const std::string key, int index = -1);

    lua_State* getLuaState();

    void pushNil();
    void push(bool b);
    void push(double num);
    void push(const std::string &str);

    void remove(int index);
    void pop(int n = 1);
    void popTopValueAsGlobal(const std::string &name);
    void setStackSize(int size);
    void setGlobalAsTop(const std::string &name);

    bool call(const std::string &func, std::initializer_list<double> numList = {}, std::initializer_list<const char *> strList = {}, int resultN = 1);

    bool load(std::string path);
    bool load(const unsigned char *buff, size_t size, const std::string &name);
    void free();

    int absIndex(int idx);

private:
    std::shared_ptr<lua_State> lua_;
};

void swap(RLuaScript &rls1, RLuaScript &rls2);

#endif // RLUASCRIPT_H
