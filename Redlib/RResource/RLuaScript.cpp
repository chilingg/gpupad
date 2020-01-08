#include "RLuaScript.h"

#include "RDebug.h"

RLuaScript::RLuaScript():
    RResource("LuaScript")
{

}

RLuaScript::RLuaScript(const std::string &path, const std::string &name):
    RResource(name)
{
    load(path);
}

RLuaScript::RLuaScript(const unsigned char *buff, size_t size, const std::string &name):
    RResource(name)
{
    load(buff, size, name);
}

RLuaScript::RLuaScript(const RLuaScript &rls):
    RResource(rls),
    lua_(rls.lua_)
{

}

RLuaScript::RLuaScript(const RLuaScript &&rls):
    RResource(std::move(rls)),
    lua_(std::move(rls.lua_))
{

}

RLuaScript &RLuaScript::operator=(RLuaScript rls)
{
    swap(rls);
    return *this;
}

RLuaScript::~RLuaScript()
{

}

void RLuaScript::swap(RLuaScript &rls)
{
    RResource::swap(rls);
    lua_.swap(rls.lua_);
}

bool RLuaScript::isValid() const
{
    return lua_ != nullptr;
}

bool RLuaScript::isValidIndex(int index) const
{
    assert(lua_ != nullptr);
    return !lua_isnone(lua_.get(), index);
}

RLuaType RLuaScript::valueType(int index) const
{
    assert(lua_ != nullptr);
    return static_cast<RLuaType>(lua_type(lua_.get(), index));
}

std::string RLuaScript::typeName(RLuaType type)
{
    assert(lua_ != nullptr);
    return lua_typename(lua_.get(), static_cast<int>(type));
}

int RLuaScript::stackSize() const
{
    assert(lua_ != nullptr);
    return lua_gettop(lua_.get());
}

bool RLuaScript::valueIsBool(int index) const
{
    assert(lua_ != nullptr);
    return lua_isboolean(lua_.get(), index);
}

bool RLuaScript::valueIsFunction(int index) const
{
    assert(lua_ != nullptr);
    return lua_isfunction(lua_.get(), index);
}

bool RLuaScript::valueIsNumber(int index) const
{
    assert(lua_ != nullptr);
    return lua_isinteger(lua_.get(), index);
}

bool RLuaScript::valueIsString(int index) const
{
    assert(lua_ != nullptr);
    return lua_isstring(lua_.get(), index);
}

bool RLuaScript::valueIsTable(int index) const
{
    assert(lua_ != nullptr);
    return lua_istable(lua_.get(), index);
}

bool RLuaScript::getBool(int index)
{
    assert(lua_ != nullptr);
    return lua_toboolean(lua_.get(), index);
}

double RLuaScript::getNumber(int index)
{
    assert(lua_ != nullptr);
    return lua_tonumber(lua_.get(), index);
}

long long RLuaScript::getInteger(int index)
{
    assert(lua_ != nullptr);
    return lua_tointeger(lua_.get(), index);
}

std::string RLuaScript::getString(int index)
{
    assert(lua_ != nullptr);
    return std::string(lua_tostring(lua_.get(), index));
}

long long RLuaScript::getTableValueToInt(const std::string key, int index)
{
    assert(lua_ != nullptr);
    lua_getfield(lua_.get(), index, key.c_str());
    long long n = lua_tointeger(lua_.get(), -1);
    lua_pop(lua_.get(), 1);

    return n;
}

double RLuaScript::getTableValueToDouble(const std::string key, int index)
{
    assert(lua_ != nullptr);
    lua_getfield(lua_.get(), index, key.c_str());
    double n = lua_tonumber(lua_.get(), -1);
    lua_pop(lua_.get(), 1);

    return n;
}

std::string RLuaScript::getTableValueToString(const std::string key, int index)
{
    assert(lua_ != nullptr);
    lua_getfield(lua_.get(), index, key.c_str());
    std::string str = lua_tostring(lua_.get(), -1);
    lua_pop(lua_.get(), 1);

    return str;
}

lua_State *RLuaScript::getLuaState()
{
    return lua_.get();
}

bool RLuaScript::load(std::string path)
{
    path = RResource::checkFilePath(path);
    lua_ .reset(luaL_newstate(), lua_close);
    luaL_openlibs(lua_.get());
    if(luaL_loadfile(lua_.get(), path.c_str()) || lua_pcall(lua_.get(), 0, 0, 0))
    {
        printError(lua_tostring(lua_.get(), -1));
        pop();
        lua_.reset();
    }

    return lua_ != nullptr;
}

bool RLuaScript::load(const unsigned char *buff, size_t size, const std::string &name)
{
    lua_ .reset(luaL_newstate(), lua_close);
    luaL_openlibs(lua_.get());
    if(luaL_loadbuffer(lua_.get(), reinterpret_cast<const char*>(buff), size, name.c_str()) || lua_pcall(lua_.get(), 0, 0, 0))
    {
        printError(lua_tostring(lua_.get(), -1));
        pop();
        lua_.reset();
    }

    return lua_ != nullptr;
}

void RLuaScript::free()
{
    lua_.reset();
}

void RLuaScript::pushNil()
{
    assert(lua_ != nullptr);
    lua_pushnil(lua_.get());
}

void RLuaScript::push(bool b)
{
    assert(lua_ != nullptr);
    lua_pushboolean(lua_.get(), b);
}

void RLuaScript::push(double num)
{
    assert(lua_ != nullptr);
    lua_pushnumber(lua_.get(), num);
}

void RLuaScript::push(const std::string &str)
{
    assert(lua_ != nullptr);
    lua_pushstring(lua_.get(), str.c_str());
}

void RLuaScript::pop(int n)
{
    assert(lua_ != nullptr);
    lua_pop(lua_.get(), n);
}

void RLuaScript::remove(int index)
{
    assert(lua_ != nullptr);
    lua_remove(lua_.get(), index);
}

void RLuaScript::setStackSize(int size)
{
    assert(lua_ != nullptr);
    lua_settop(lua_.get(), size);
}

void RLuaScript::popTopValueAsGlobal(const std::string &name)
{
    assert(lua_ != nullptr);
    lua_setglobal(lua_.get(), name.c_str());
}

void RLuaScript::setGlobalAsTop(const std::string &name)
{
    assert(lua_ != nullptr);
    lua_getglobal(lua_.get(), name.c_str());
}

bool RLuaScript::call(const std::string &func, std::initializer_list<double> numList, std::initializer_list<const char *> strList, int resultN)
{
    assert(lua_ != nullptr);
    lua_getglobal(lua_.get(), func.c_str());
    for(auto n : numList)
        lua_pushnumber(lua_.get(), n);
    for(auto s : strList)
        lua_pushstring(lua_.get(), s);

    if(lua_pcall(lua_.get(), numList.size()+strList.size(), resultN, 0) != 0)
    {
#ifdef R_DEBUG
        printError("Failure call in function <" + func + ">: " + lua_tostring(lua_.get(), -1));
#endif
        lua_pop(lua_.get(), -1);
        return false;
    }

    return true;
}

int RLuaScript::absIndex(int idx)
{
    assert(lua_ != nullptr);
    return lua_absindex (lua_.get(), idx);
}

void swap(RLuaScript &rls1, RLuaScript &rls2)
{
    rls1.swap(rls2);
}
