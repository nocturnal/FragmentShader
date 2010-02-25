#include "luavalid.h"

LuaValidator::LuaValidator(lua_State *L, int getter, int setter, int validator)
{
    luaL_checktype(L, setter, LUA_TFUNCTION);
    luaL_checktype(L, getter, LUA_TFUNCTION);
    luaL_checktype(L, validator, LUA_TFUNCTION);
    this->L = L;
    lua_pushvalue(L, setter);
    this->setter = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_pushvalue(L, getter);
    this->getter = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_pushvalue(L, validator);
    this->validator = luaL_ref(L, LUA_REGISTRYINDEX);
}

LuaValidator::~LuaValidator()
{
    luaL_unref(L, LUA_REGISTRYINDEX, validator);
    luaL_unref(L, LUA_REGISTRYINDEX, setter);
    luaL_unref(L, LUA_REGISTRYINDEX, getter);
}

void
LuaValidator::PushValue(wxVariant& value)
{
    wxString type = value.GetType();
    if (type == wxT("bool"))
        lua_pushboolean(L, value.GetBool());
    else if (type == wxT("string"))
        lua_pushstring(L, (const char *)value.GetString().c_str());
    else if (type == wxT("double"))
    {
        lua_pushnumber(L, value.GetDouble());
        lua_tostring(L, -1);
    }
    else if (type == wxT("long"))
    {
        lua_pushnumber(L, value.GetLong());
        lua_tostring(L, -1);
    }
    else
        lua_pushnil(L);
}

void
LuaValidator::SetValue(wxVariant& value)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, setter);
    PushValue(value);
    lua_call(L, 1, 0);
}
    
wxVariant
LuaValidator::GetValue()
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, getter);
    lua_call(L, 0, 1);
    wxVariant value;
    switch (lua_type(L, -1))
    {
        case LUA_TBOOLEAN:
            value = wxVariant((bool)lua_toboolean(L, -1));
            break;
        case LUA_TNUMBER:
        case LUA_TSTRING:
            value = wxVariant(lua_tostring(L, -1));
            break;
    }
    lua_pop(L, 1);
    return value;
}

bool
LuaValidator::Validate(wxVariant& value)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, validator);
    PushValue(value);
    lua_call(L, 1, 1);
    bool ok = lua_toboolean(L, -1);
    lua_pop(L, 1);
    return ok;
}

void
LuaEnumValidator::SetValue(wxVariant& value)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, setter);
    lua_pushnumber(L, value.GetLong());
    lua_call(L, 1, 0);
}

wxVariant
LuaEnumValidator::GetValue()
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, getter);
    lua_call(L, 0, 1);
    wxVariant value((long)lua_tonumber(L, -1));
    lua_pop(L, 1);
    return value;
}

bool
LuaEnumValidator::Validate(wxVariant& value)
{
    return true;
}
