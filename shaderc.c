#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "expr.h"

/* Must be a power of two. */
#define CHUNK_SIZE 1024

#ifndef MAX_PATH
#define MAX_PATH 1024
#endif

typedef struct
{
	char *data;
	int length, size;
} string_t;

static void
str_init(string_t *str)
{
	str->data = NULL;
	str->length = str->size = 0;
}

static string_t *
str_new(void)
{
	string_t *str = malloc(sizeof(string_t));
	if (str == NULL)
		return NULL;
	str_init(str);
	return str;
}

static int
str_copy(string_t *str, const char *data)
{
	int length = strlen(data);
	if ((length + 1) > str->size)
	{
		int size = (length + 1 + (CHUNK_SIZE - 1)) & ~(CHUNK_SIZE - 1);
		char *new_data = (char *)realloc(str->data, size);
		if (new_data == NULL)
			return 0;
		str->data = new_data;
		str->size = size;
	}
	strcpy(str->data, data);
	str->length = length;
	return 1;
}

static int
str_concat(string_t *str, const char *data)
{
	int length = strlen(data);
	if ((str->length + length + 1) > str->size)
	{
		int size = (str->length + length + 1 + (CHUNK_SIZE - 1)) & ~(CHUNK_SIZE - 1);
		char *new_data = (char *)realloc(str->data, size);
		if (new_data == NULL)
			return 0;
		str->data = new_data;
		str->size = size;
	}
	strcpy(str->data + str->length, data);
	str->length += length;
	return 1;
}

static inline int
str_length(string_t *str)
{
	return str->length;
}

static inline const char *
str_data(string_t *str)
{
	return str->data;
}

static inline char
str_lastchar(string_t *str)
{
	if (str->length == 0)
		return '\0';
	return str->data[str->length - 1];
}

static inline void
str_eatlast(string_t *str)
{
	if (str->length != 0)
		str->data[--str->length] = '\0';
}		

static void
str_free(string_t *str)
{
	if (str != NULL)
	{
		free(str->data);
		str_init(str);
	}
}

typedef struct
{
	const char *dot_app;
	string_t *dot, *xdot, *code;
} udata_t;

static int
Dot2XDot(udata_t *udata)
{
	if (udata->xdot == NULL)
	{
		udata->xdot = str_new();
		if (udata->xdot == NULL)
			return 0;
	}
	else
		str_copy(udata->xdot, "");
	FILE *file = fopen("C:\\temp.dot", "wb");
	if (file == NULL)
		return 0;
	if (fwrite(str_data(udata->dot), 1, str_length(udata->dot), file) != str_length(udata->dot))
	{
		fclose(file);
		return 0;
	}
	fclose(file);
	file = popen(udata->dot_app, "rb");
	if (file == NULL)
	{
		oops:
		unlink("C:\\temp.dot");
		return 0;
	}
	char line[1024];
	while (fgets(line, sizeof(line), file) != NULL)
	{
		line[strlen(line) - 2] = '\0';
		int length = str_length(udata->xdot);
		if (length != 0 && str_lastchar(udata->xdot) == '\\')
			str_eatlast(udata->xdot);
		else
			if (!str_concat(udata->xdot, "\n"))
			{
				fclose(file);
				goto oops;
			}
		if (!str_concat(udata->xdot, line))
		{
			fclose(file);
			goto oops;
		}
	}
	fclose(file);
	unlink("C:\\temp.dot");
	return 1;
}

static int
set_dot_data(lua_State *L)
{
	udata_t *udata = (udata_t *)lua_touserdata(L, lua_upvalueindex(1));
	const char *dot = lua_tostring(L, 1);
	if (udata->dot == NULL)
	{
		udata->dot = str_new();
		if (udata->dot == NULL)
			return 0;
	}
	str_copy(udata->dot, dot);
	return 0;
}

static int
get_xdot_data(lua_State *L)
{
	udata_t *udata = (udata_t *)lua_touserdata(L, lua_upvalueindex(1));
	if (!Dot2XDot(udata))
		return 0;
	lua_pushlstring(L, str_data(udata->xdot), str_length(udata->xdot));
	return 1;
}

static int
set_code(lua_State *L)
{
	udata_t *udata = (udata_t *)lua_touserdata(L, lua_upvalueindex(1));
	if (udata->code == NULL)
	{
		udata->code = str_new();
		if (udata->code == NULL)
			return 0;
	}
	int index;
	for (index = 1; lua_isstring(L, index); index++)
		str_concat(udata->code, lua_tostring(L, index));
	return 0;
}

static int
add_code(lua_State *L)
{
	udata_t *udata = (udata_t *)lua_touserdata(L, lua_upvalueindex(1));
	if (udata->code == NULL)
	{
		udata->code = str_new();
		if (udata->code == NULL)
			return 0;
	}
	int index;
	for (index = 1; lua_isstring(L, index); index++)
		str_concat(udata->code, lua_tostring(L, index));
	return 0;
}

static int
dummy(lua_State *L)
{
	return 0;
}

static int
open_io(lua_State *L)
{
	luaopen_io(L);
	return 0;
}

int
load_lua(lua_State *L, const char *name, int nresults)
{
	int res = luaL_loadfile(L, name);
	if (res != 0)
	{
		return res;
	}
	return lua_pcall(L, 0, nresults, 0);
}

static expr_function_t *
get_function(void *user_data, const char *name)
{
	return NULL;
}

static expr_variable_t *
get_variable(void *user_data, const char *name)
{
	return NULL;
}

static int
evaluate(lua_State *L)
{
	const char *expr = luaL_checkstring(L, 1);
	expr_config_t config;
	config.get_function = get_function;
	config.get_variable = get_variable;
	double result;
	if (expr_evaluate_string(&config, expr, &result) != EXPR_OK)
		return 0;
	lua_pushnumber(L, result);
	return 1;
}

static int
floatbytes(lua_State *L)
{
	float f = (float)luaL_checknumber(L, 1);
	unsigned char *u = (unsigned char *)&f;
	lua_pushnumber(L, u[3]);
	lua_pushnumber(L, u[2]);
	lua_pushnumber(L, u[1]);
	lua_pushnumber(L, u[0]);
	return 4;
}

lua_State *
luaB_openlua()
{
	lua_State *L = luaL_newstate();
	luaopen_base(L);
	luaopen_string(L);
	luaopen_math(L);
	luaopen_os(L);
	luaopen_table(L);
	luaopen_debug(L);
	// hum... luaopen_io is failing when called directly. dunno why.
	//luaopen_io(L);
	lua_pushcfunction(L, open_io);
	lua_call(L, 0, 0);
	lua_getglobal(L, "math");
	lua_pushcfunction(L, evaluate);
	lua_setfield(L, -2, "evaluate");
	lua_pushcfunction(L, floatbytes);
	lua_setfield(L, -2, "floatbytes");
	
	//#include "class.lua.h"
	//#include "graph.lua.h"
	//#include "functions.lua.h"
	//#include "pickle.lua.h"
	if (
		load_lua(L, "C:\\PS3\\Insomniac\\projects\\shader\\src\\class.lua", 0) != 0 ||
		load_lua(L, "C:\\PS3\\Insomniac\\projects\\shader\\src\\graph.lua", 0) != 0 ||
		load_lua(L, "C:\\PS3\\Insomniac\\projects\\shader\\src\\functions.lua", 0) != 0 ||
		load_lua(L, "C:\\PS3\\Insomniac\\projects\\shader\\src\\pickle.lua", 0) != 0
	) {
		printf("Error: %s\n", lua_tostring(L, -1));
		lua_close(L);
		return NULL;
	}
	
	lua_settop(L, 0);
	return L;
}

void
luaB_setfuncs(lua_State *L, udata_t *udata)
{
	lua_newtable(L);
	
	lua_pushcclosure(L, dummy, 0);
	lua_setfield(L, -2, "menu_ctrl");
	
	lua_pushcclosure(L, dummy, 0);
	lua_setfield(L, -2, "set_status");

	lua_pushlightuserdata(L, (void *)udata);
	lua_pushcclosure(L, set_dot_data, 1);
	lua_setfield(L, -2, "set_dot_data");
	
	lua_pushlightuserdata(L, (void *)udata);
	lua_pushcclosure(L, get_xdot_data, 1);
	lua_setfield(L, -2, "get_xdot_data");
	
	lua_pushcclosure(L, dummy, 0);
	lua_setfield(L, -2, "graph_redraw");
	
	lua_pushlightuserdata(L, (void *)udata);
	lua_pushcclosure(L, set_code, 1);
	lua_setfield(L, -2, "set_code");

	lua_pushlightuserdata(L, (void *)udata);
	lua_pushcclosure(L, add_code, 1);
	lua_setfield(L, -2, "add_code");
	
	lua_pushcclosure(L, dummy, 0);
	lua_setfield(L, -2, "popup");
	
	lua_pushcclosure(L, dummy, 0);
	lua_setfield(L, -2, "set_tooltip");
	
	lua_pushcclosure(L, dummy, 0);
	lua_setfield(L, -2, "props_clear");

	lua_pushcclosure(L, dummy, 0);
	lua_setfield(L, -2, "props_delete");

	lua_pushcclosure(L, dummy, 0);
	lua_setfield(L, -2, "props_redraw");

	lua_pushcclosure(L, dummy, 0);
	lua_setfield(L, -2, "props_text_input");

	lua_pushcclosure(L, dummy, 0);
	lua_setfield(L, -2, "props_memo_input");

	lua_pushcclosure(L, dummy, 0);
	lua_setfield(L, -2, "props_check_box");

	lua_pushcclosure(L, dummy, 0);
	lua_setfield(L, -2, "props_list");
	
	lua_pushcclosure(L, dummy, 0);
	lua_setfield(L, -2, "set_modified");
	
	lua_pushcclosure(L, dummy, 0);
	lua_setfield(L, -2, "set_page");
	
	lua_setglobal(L, "c_lib");
}

static inline int
call(lua_State *L, int nargs, int nresults)
{
	int top = lua_gettop(L);
	int res = lua_pcall(L, nargs, nresults, 0);
	if (res != 0)
	{
		printf("Error: %s\n", lua_tostring(L, -1));
		lua_settop(L, top - nargs - 1 + nresults);
		return res;
	}
	return 0;
}

int
open_libs(lua_State *L, const char *argv0)
{
	char lib_path[MAX_PATH];
	strncpy(lib_path, argv0, MAX_PATH);
	lib_path[MAX_PATH - 1] = '\0';
	*(strrchr(lib_path, '\\') + 1) = '\0';
	strncat(lib_path, "lib\\", MAX_PATH);
	lib_path[MAX_PATH - 1] = '\0';

	DIR *dir = opendir(lib_path);
	if (dir == NULL)
	{
		printf("Error: %s\n", strerror(errno));
		return 0;
	}
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_name[0] == '.')
			continue;
		char filename[MAX_PATH];
		strcpy(filename, lib_path);
		strncat(filename, entry->d_name, MAX_PATH);
		filename[MAX_PATH - 1] = '\0';
		if (load_lua(L, filename, 2) != 0)
		{
			printf("Error: %s\n", lua_tostring(L, -1));
			return 0;
		}
		lua_setglobal(L, lua_tostring(L, -2));
		lua_pop(L, 1);
	}
	return 1;
}

int
main(int argc, const char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s input.ext\n\tThe output is generated at stdout.\n", argv[0]);
		return 0;
	}
	
	lua_State *L = luaB_openlua();
	//luaL_dostring(L, "function __debug(event, line) local info = debug.getinfo(2, 'nSl') if info.name ~= '(for generator)' then print(info.name or '?', info.short_src, info.currentline) end end debug.sethook(__debug, 'c')");
	udata_t udata;
	udata.dot = NULL;
	udata.xdot = NULL;
	udata.code = NULL;
	luaB_setfuncs(L, &udata);
	if (!open_libs(L, argv[0]))
		exit(-1);

	char dot_app[MAX_PATH] = { '"' };
	strncat(dot_app, argv[0], MAX_PATH);
	dot_app[MAX_PATH - 1] = '\0';
	*(strrchr(dot_app, '\\') + 1) = '\0';
	strncat(dot_app, "dot\\dot.exe\" -Txdot -y C:\\temp.dot", MAX_PATH);
	dot_app[MAX_PATH - 1] = '\0';
	udata.dot_app = dot_app;

	lua_getglobal(L, "open");
	lua_pushstring(L, argv[1]);
	call(L, 1, 0);

	printf("%s", str_data(udata.code));
	
	return 0;
}
