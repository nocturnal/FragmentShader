#include "debug.h"

#include <wx/wx.h>

#include <map>

#include <windows.h>
#include <stdio.h>
#include <stdarg.h>

#include "shape.h"
#include "luautil.h"

namespace Debug
{
  static const char *Format(const char *fmt, va_list args)
  {
    static char buffer[1024];
    ::vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, fmt, args);
    buffer[sizeof(buffer) - 1] = 0;
    return buffer;
  }

  const char *Format(const char *fmt, ...)
  {
    va_list args;
    va_start(args, fmt);
    const char *res = Format(fmt, args);
    va_end(args);
    return res;
  }

#ifdef _DEBUG

	static HANDLE console;
	static HANDLE log;

	void *
	DumpHeap(const char *op, void *ptr, size_t size, const char *file, int line)
	{
		Debug::Printf("%s\t%p\t%d\t%s\t%d\n", op, ptr, size, file, line);
		return ptr;
	}

	static int
	LuaAssert(lua_State *L)
	{
		if (!lua_toboolean(L, 1))
		{
			if (!lua_isstring(L, 2))
			{
				lua_pushliteral(L, "assert failed");
			}
			lua_error(L);
		}
		return 0;
	}

	static void
	PushObjectName(lua_State *L, int obj, int table)
	{
		const static char find[] =
			"local unknown = nil\n"
			"local function find(object, table, visited)\n"
			"  if visited[table] then\n"
			"    return unknown\n"
			"  end\n"
			"  visited[table] = true\n"
			"  for key, value in pairs(table) do\n"
			"    if value == object then\n"
			"      return tostring(key)\n"
			"    end\n"
			"    if type(value) == 'table' then\n"
			"      local name = find(object, value, visited)\n"
			"      if name ~= unknown then\n"
			"        return name\n"
			"      end\n"
			"    end\n"
			"  end\n"
			"  return unknown\n"
			"end\n"
			"return function(object, table)\n"
			"  local name = find(object, table, {})\n"
			"  if name == unknown then\n"
			"    if type(object) == 'table' and object.class_name then\n"
			"      local class = object.class_name()\n"
			"      if class then\n"
			"        name = 'instance_of ' .. class\n"
			"      end\n"
			"    end\n"
			"  end\n"
			"  return name\n"
			"end\n";
		LuaUtil::LoadBuffer(L, find, sizeof(find) - 1, "find");
		lua_call(L, 0, 1);
		lua_pushvalue(L, obj);
		lua_pushvalue(L, table);
		lua_call(L, 2, 1);
	}

	static void
	LuaStringfy(lua_State *L, bool dump)
	{
		int top = lua_gettop(L);
		for (int index = 1; index <= top; index++)
		{
			if (dump)
			{
				lua_pushfstring(L, "%d: ", index);
			}
			switch (lua_type(L, index))
			{
			case LUA_TNIL:
				lua_pushliteral(L, "nil");
				break;
			case LUA_TNUMBER:
				lua_pushvalue(L, index);
				lua_tostring(L, -1);
				break;
			case LUA_TBOOLEAN:
				if (lua_toboolean(L, index))
				{
					lua_pushliteral(L, "true");
				}
				else
				{
					lua_pushliteral(L, "false");
				}
				break;
			case LUA_TSTRING:
				lua_pushvalue(L, index);
				break;
			case LUA_TTABLE:
				PushObjectName(L, index, LUA_GLOBALSINDEX);
				if (lua_isnil(L, -1))
				{
					lua_pop(L, 1);
					lua_pushfstring(L, "table@%p", lua_topointer(L, index));
				}
				break;
			case LUA_TFUNCTION:
				lua_pushfstring(L, "function@%p", lua_topointer(L, index));
				break;
			case LUA_TUSERDATA:
			case LUA_TLIGHTUSERDATA:
				lua_pushfstring(L, "userdata@%p", lua_topointer(L, index));
				break;
			case LUA_TTHREAD:
				lua_pushfstring(L, "thread@%p", lua_topointer(L, index));
				break;
			}
			if (dump)
			{
				lua_pushliteral(L, "\n");
			}
		}
		lua_concat(L, lua_gettop(L) - top);
	}

	static int
	LuaPrint(lua_State *L)
	{
		LuaStringfy(L, false);
		Printf("%s", lua_tostring(L, -1));
		lua_pop(L, 1);
		return 0;
	}

	static int
	LuaMessageBox(lua_State *L)
	{
		LuaStringfy(L, false);
		wxMessageBox(lua_tostring(L, -1), "Debug");
		lua_pop(L, 1);
		return 0;
	}

	void
	Init(lua_State *L)
	{
		// Allocate a console.
		AllocConsole();
		console = GetStdHandle(STD_OUTPUT_HANDLE);
		// Create the log file.
		log = CreateFile(wxT("log.txt"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		// Register global Lua functions.
		lua_pushcfunction(L, LuaAssert);
		lua_setglobal(L, "assert");
		lua_pushcfunction(L, LuaPrint);
		lua_setglobal(L, "print");
		lua_pushcfunction(L, LuaMessageBox);
		lua_setglobal(L, "message_box");
		// Function to dump a table.
		const static char dump_lua[] =
			"function dump(table)\n"
			"  if type(table) ~= 'table' then\n"
			"    return\n"
			"  end\n"
			"  local msg = ''\n"
			"  for key, value in pairs(table) do\n"
			"    msg = msg .. tostring(key)  .. ' = ' .. tostring(value) .. '\\n'\n"
			"  end\n"
			"  return msg\n"
			"end\n";
		LuaUtil::LoadBuffer(L, dump_lua, sizeof(dump_lua) - 1, "dump");
		LuaUtil::Call(L, 0, 0);
	}

	void Printf(const char *fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		const char *res = Format(fmt, args);
		va_end(args);
		DWORD len = (DWORD)strlen(res);
		DWORD numwritten;
		WriteFile(console, res, len, &numwritten, NULL);
		WriteFile(log, res, len, &numwritten, NULL);
	}

	void DumpLuaStack(lua_State *L)
	{
		LuaStringfy(L, true);
		Printf("%s", lua_tostring(L, -1));
		lua_pop(L, 1);
	}

	void Save(Shape *shape, const char *file)
	{
		wxXmlDocument doc;
		doc.SetRoot(shape->Serialize());
		doc.Save(wxString(file));
	}

#else

	static int
	LuaDummy(lua_State *L)
	{
		return 0;
	}

	void
	Init(lua_State *L)
	{
		lua_pushcfunction(L, LuaDummy);
		lua_setglobal(L, "assert");
		lua_pushcfunction(L, LuaDummy);
		lua_setglobal(L, "print");
		lua_pushcfunction(L, LuaDummy);
		lua_setglobal(L, "message_box");
	}

#endif

};
