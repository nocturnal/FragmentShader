#ifndef __LUAVALID_H__
#define __LUAVALID_H__

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <wx/wx.h>
#include "propedit.h"

class LuaValidator: public wxPropertyValidator
{
	public:
		LuaValidator(lua_State *L, int getter, int setter, int validator);
		~LuaValidator();

		void SetValue(wxVariant& value);
		wxVariant GetValue();
		bool Validate(wxVariant& value);
	
	protected:
		void PushValue(wxVariant& value);
	
		lua_State *L;
		int setter, getter, validator;
};

class LuaEnumValidator: public LuaValidator
{
	public:
		LuaEnumValidator(lua_State *L, int getter, int setter, int validator) : LuaValidator(L, getter, setter, validator) {};

		void SetValue(wxVariant& value);
		wxVariant GetValue();
		bool Validate(wxVariant& value);
};

#endif /* __LUAVALID_H__ */
