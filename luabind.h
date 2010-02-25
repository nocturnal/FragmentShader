#ifndef __LUABIND_H__
#define __LUABIND_H__

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/notebook.h>
#include "graph.h"
#include "propedit.h"

int load_lua(lua_State *L, const char *name, int nresults);
void luaB_pushdc(lua_State *L, wxDC *dc);
lua_State *luaB_openlua();
void luaB_setfuncs(lua_State *L, wxMenu *edit, wxStatusBar *status, wxPropertyEditor *props, Graph *graph, wxTextCtrl *code, wxNotebook *ph);

#endif /* __LUABIND_H__ */
