#include "luabind.h"
#include "luavalid.h"
#include "wxpopupmenu.h"

extern "C" {
#include "expr.h"
}

#define GPERF_NONE            0
#define GPERF_CLEAR            1
#define GPERF_SET_FILL_COLOR    2
#define GPERF_SET_LINE_COLOR    3
#define GPERF_DRAW_POLYGON    4
#define GPERF_DRAW_SPLINE        5
#define GPERF_DRAW_LINE        6
#define GPERF_DRAW_RECTANGLE    7
#define GPERF_INVERT            8
#define GPERF_LIST            9

#include "gperf.h"

static int
open_io(lua_State *L)
{
    luaopen_io(L);
    return 0;
}

static int
menu_ctrl(lua_State *L)
{
    wxMenu *self = (wxMenu *)lua_touserdata(L, lua_upvalueindex(1));
    const char *item = luaL_checkstring(L, 1);
    int enable = lua_toboolean(L, 2);
    if (!strcmp(item, "copy"))
    {
        self->Enable(wxID_CUT, enable);
        self->Enable(wxID_COPY, enable);
        self->Enable(wxID_DUPLICATE, enable);
    }
    else if (!strcmp(item, "paste"))
        self->Enable(wxID_PASTE, enable);
    else if (!strcmp(item, "undo"))
        self->Enable(wxID_UNDO, enable);
    else if (!strcmp(item, "redo"))
        self->Enable(wxID_REDO, enable);
    else
        luaL_error(L, "Unknown menu item: %s", item);
    return 0;
}

static int
set_status(lua_State *L)
{
    wxStatusBar *self = (wxStatusBar *)lua_touserdata(L, lua_upvalueindex(1));
    int pos = luaL_checkinteger(L, 1);
    lua_concat(L, lua_gettop(L) - 1);
    const char *text = luaL_checkstring(L, 2);
    self->SetStatusText(wxString::FromAscii(text), pos - 1);
    return 0;
}

static int
set_dot_data(lua_State *L)
{
    Graph *self = (Graph *)lua_touserdata(L, lua_upvalueindex(1));
    const char *dotdata = lua_tostring(L, 1);
    self->SetDotData(dotdata);
    return 0;
}

static int
get_xdot_data(lua_State *L)
{
    Graph *self = (Graph *)lua_touserdata(L, lua_upvalueindex(1));
    const char *xdot = self->GetXDotData();
    lua_pushstring(L, xdot);
    return 1;
}

static int
redraw(lua_State *L)
{
    Graph *self = (Graph *)lua_touserdata(L, lua_upvalueindex(1));
    self->Redraw(true);
    return 0;
}

static int
set_code(lua_State *L)
{
    wxTextCtrl *self = (wxTextCtrl *)lua_touserdata(L, lua_upvalueindex(1));
    self->Clear();
    for (int index = 1; lua_isstring(L, index); index++)
        self->AppendText(wxString::FromAscii(lua_tostring(L, index)));
    return 0;
}

static int
add_code(lua_State *L)
{
    wxTextCtrl *self = (wxTextCtrl *)lua_touserdata(L, lua_upvalueindex(1));
    for (int index = 1; lua_isstring(L, index); index++)
        self->AppendText(wxString::FromAscii(lua_tostring(L, index)));
    return 0;
}

static int
popup(lua_State *L)
{
    Graph *self = (Graph *)lua_touserdata(L, lua_upvalueindex(1));
    wxPopupMenu menu;
    int top = lua_gettop(L);
    for (int index = 1; index <= top; index++)
        if (lua_isstring(L, index))
            menu.Append(menu.GetNextId(), wxString::FromAscii(lua_tostring(L, index)));
        else
            menu.Append(menu.GetNextId(), wxT(""), wxT(""), wxITEM_SEPARATOR);
    lua_pushnumber(L, menu.Popup(self) + 1);
    return 1;
}

static int
set_tooltip(lua_State *L)
{
    Graph *self = (Graph *)lua_touserdata(L, lua_upvalueindex(1));
    const char *tip = luaL_checkstring(L, 1);
    wxToolTip *tooltip = self->GetToolTip();
    tooltip->SetTip(wxString::FromAscii(tip));
    return 0;
}


static int
props_clear(lua_State *L)
{
    wxPropertyEditor *self = (wxPropertyEditor *)lua_touserdata(L, lua_upvalueindex(1));
    self->Clear();
    return 0;
}

static int
props_delete(lua_State *L)
{
    wxPropertyEditor *self = (wxPropertyEditor *)lua_touserdata(L, lua_upvalueindex(1));
    const char *label = luaL_checkstring(L, 1);
    self->Delete(wxString::FromAscii(label));
    return 0;
}

static int
props_redraw(lua_State *L)
{
    wxPropertyEditor *self = (wxPropertyEditor *)lua_touserdata(L, lua_upvalueindex(1));
    self->Refresh();
    return 0;
}

static int
props_text_input(lua_State *L)
{
    wxPropertyEditor *self = (wxPropertyEditor *)lua_touserdata(L, lua_upvalueindex(1));
    const char *label = luaL_checkstring(L, 1);
    luaL_checktype(L, 2, LUA_TFUNCTION);
    luaL_checktype(L, 3, LUA_TFUNCTION);
    luaL_checktype(L, 4, LUA_TFUNCTION);
    wxPGId id = self->Append(wxStringProperty(wxString::FromAscii(label), wxPG_LABEL));
    LuaValidator *v = new LuaValidator(L, 2, 3, 4);
    self->SetPropertyValidator(id, v);
    return 0;
}

static int
props_memo_input(lua_State *L)
{
    wxPropertyEditor *self = (wxPropertyEditor *)lua_touserdata(L, lua_upvalueindex(1));
    const char *label = luaL_checkstring(L, 1);
    luaL_checktype(L, 2, LUA_TFUNCTION);
    luaL_checktype(L, 3, LUA_TFUNCTION);
    luaL_checktype(L, 4, LUA_TFUNCTION);
    wxPGId id = self->Append(wxLongStringProperty(wxString::FromAscii(label), wxPG_LABEL));
    LuaValidator *v = new LuaValidator(L, 2, 3, 4);
    self->SetPropertyValidator(id, v);
    return 0;
}

static int
props_check_box(lua_State *L)
{
    wxPropertyEditor *self = (wxPropertyEditor *)lua_touserdata(L, lua_upvalueindex(1));
    const char *label = luaL_checkstring(L, 1);
    luaL_checktype(L, 2, LUA_TFUNCTION);
    luaL_checktype(L, 3, LUA_TFUNCTION);
    luaL_checktype(L, 4, LUA_TFUNCTION);
    wxPGId id = self->Append(wxBoolProperty(wxString::FromAscii(label), wxPG_LABEL));
    self->SetPropertyValidator(id, new LuaValidator(L, 2, 3, 4));
    self->SetPropertyAttribute(id, wxPG_BOOL_USE_CHECKBOX, (long)1, wxPG_RECURSE);
    //self->SetPropertyAttributeAll(wxPG_BOOL_USE_CHECKBOX, (long)1);
    return 0;
}

static int
props_list(lua_State *L)
{
    wxPropertyEditor *self = (wxPropertyEditor *)lua_touserdata(L, lua_upvalueindex(1));
    const char *label = luaL_checkstring(L, 1);
    luaL_checktype(L, 2, LUA_TTABLE);
    luaL_checktype(L, 3, LUA_TFUNCTION);
    luaL_checktype(L, 4, LUA_TFUNCTION);
    luaL_checktype(L, 5, LUA_TFUNCTION);
    wxPGChoices choices;
    for (int index = 1;; index++)
    {
        lua_pushnumber(L, index);
        lua_gettable(L, 2);
        if (!lua_isstring(L, -1))
        {
            lua_pop(L, 1);
            break;
        }
        choices.Add(wxString::FromAscii(lua_tostring(L, -1)), index);
        lua_pop(L, 1);
    }
    wxPGId id = self->Append(wxEnumProperty(wxString::FromAscii(label), wxPG_LABEL, choices));
    self->SetPropertyValidator(id, new LuaEnumValidator(L, 3, 4, 5));
    return 0;
}

static int
set_modified(lua_State *L)
{
    Graph *self = (Graph *)lua_touserdata(L, lua_upvalueindex(1));
    bool modified = lua_toboolean(L, 1);
    self->SetModified(modified);
    return 0;
}

static int
set_page(lua_State *L)
{
    wxNotebook *self = (wxNotebook *)lua_touserdata(L, lua_upvalueindex(1));
    const char *name = luaL_checkstring(L, 1);
    if (!strcmp(name, "props"))
        self->ChangeSelection(0);
    else if (!strcmp(name, "help"))
        self->ChangeSelection(1);
    else
        luaL_error(L, "Invalid page name: %s", name);
    return 0;
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

static int
u32bytes(lua_State *L)
{
    unsigned int i = (unsigned int)luaL_checknumber(L, 1);
    unsigned char *u = (unsigned char *)&i;
    lua_pushnumber(L, u[3]);
    lua_pushnumber(L, u[2]);
    lua_pushnumber(L, u[1]);
    lua_pushnumber(L, u[0]);
    return 4;
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

typedef struct
{
    wxDC *dc;
}
dc_t;

static int
dc_clear(lua_State *L)
{
    dc_t *self = (dc_t *)lua_touserdata(L, lua_upvalueindex(1));
    self->dc->Clear();
    return 0;
}

static int
dc_set_fill_color(lua_State *L)
{
    dc_t *self = (dc_t *)lua_touserdata(L, lua_upvalueindex(1));
    int r = luaL_checkinteger(L, 1);
    int g = luaL_checkinteger(L, 2);
    int b = luaL_checkinteger(L, 3);
    wxColour color(r, g, b);
    wxBrush brush(color, wxSOLID);
    self->dc->SetBrush(brush);
    return 0;
}

static int
dc_set_line_color(lua_State *L)
{
    dc_t *self = (dc_t *)lua_touserdata(L, lua_upvalueindex(1));
    int r = luaL_checkinteger(L, 1);
    int g = luaL_checkinteger(L, 2);
    int b = luaL_checkinteger(L, 3);
    int width = luaL_optinteger(L, 4, 0);
    if (width > 0)
    {
        wxColour color(r, g, b);
        wxPen pen(color, width, wxSOLID);
        self->dc->SetPen(pen);
    }
    else
        self->dc->SetPen(*wxTRANSPARENT_PEN);
}

static int
dc_draw_polygon(lua_State *L)
{
    dc_t *self = (dc_t *)lua_touserdata(L, lua_upvalueindex(1));
    int count = lua_gettop(L) / 2;
    wxPoint points[count];
    for (int index = 0; index < count; index++)
    {
        points[index].x = luaL_checkinteger(L, index * 2 + 1);
        points[index].y = luaL_checkinteger(L, index * 2 + 2);
    }
    self->dc->DrawPolygon(count, points);
    return 0;
}

static int
dc_draw_spline(lua_State *L)
{
    dc_t *self = (dc_t *)lua_touserdata(L, lua_upvalueindex(1));
    int count = lua_gettop(L) / 2;
    wxPoint points[count];
    for (int index = 0; index < count; index++)
    {
        points[index].x = luaL_checkinteger(L, index * 2 + 1);
        points[index].y = luaL_checkinteger(L, index * 2 + 2);
    }
    self->dc->DrawSpline(count, points);
    return 0;
}

static int
dc_draw_line(lua_State *L)
{
    dc_t *self = (dc_t *)lua_touserdata(L, lua_upvalueindex(1));
    int x0 = luaL_checkinteger(L, 1);
    int y0 = luaL_checkinteger(L, 2);
    int x1 = luaL_checkinteger(L, 3);
    int y1 = luaL_checkinteger(L, 4);
    self->dc->DrawLine(x0, y0, x1, y1);
    return 0;
}

static int
dc_draw_rectangle(lua_State *L)
{
    dc_t *self = (dc_t *)lua_touserdata(L, lua_upvalueindex(1));
    int x0 = luaL_checkinteger(L, 1);
    int y0 = luaL_checkinteger(L, 2);
    int x1 = luaL_checkinteger(L, 3);
    int y1 = luaL_checkinteger(L, 4);
    self->dc->DrawRectangle(x0, y0, x1 - x0 + 1, y1 - y0 + 1);
    return 0;
}

static int
dc_invert(lua_State *L)
{
    dc_t *self = (dc_t *)lua_touserdata(L, lua_upvalueindex(1));
    int x0 = luaL_checkinteger(L, 1);
    int y0 = luaL_checkinteger(L, 2);
    int x1 = luaL_checkinteger(L, 3);
    int y1 = luaL_checkinteger(L, 4);
    self->dc->Blit(x0, y0, x1 - x0 + 1, y1 - y0 + 1, self->dc, x0, y0, wxINVERT);
}

static int
dc_get_index(lua_State *L)
{
    size_t len;
    const char *key = luaL_checklstring(L, 2, &len);
    const field_t *field = in_word_set(key, len);
    lua_pushvalue(L, 1);
    if (field == NULL)
    {
        unknown:
        lua_pop(L, 1);
        luaL_error(L, "unknown field %s in type wxDC.", key);
    }
    switch (field == NULL ? GPERF_NONE : field->index)
    {
        case GPERF_CLEAR:
            lua_pushcclosure(L, dc_clear, 1);
            break;
        case GPERF_SET_FILL_COLOR:
            lua_pushcclosure(L, dc_set_fill_color, 1);
            break;
        case GPERF_SET_LINE_COLOR:
            lua_pushcclosure(L, dc_set_line_color, 1);
            break;
        case GPERF_DRAW_POLYGON:
            lua_pushcclosure(L, dc_draw_polygon, 1);
            break;
        case GPERF_DRAW_SPLINE:
            lua_pushcclosure(L, dc_draw_spline, 1);
            break;
        case GPERF_DRAW_LINE:
            lua_pushcclosure(L, dc_draw_line, 1);
            break;
        case GPERF_DRAW_RECTANGLE:
            lua_pushcclosure(L, dc_draw_rectangle, 1);
            break;
        case GPERF_INVERT:
            lua_pushcclosure(L, dc_invert, 1);
            break;
        default:
            goto unknown;
    }
    return 1;
}

static int
dc_gc(lua_State *L)
{
    return 0;
}

void
luaB_pushdc(lua_State *L, wxDC *dc)
{
    dc_t *self = (dc_t *)lua_newuserdata(L, sizeof(dc_t *));
    self->dc = dc;
    luaL_getmetatable(L, "wxDC");
    lua_setmetatable(L, -2);
}

static void
dc_init(lua_State *L)
{
    lua_newtable(L);
    if (luaL_newmetatable(L, "wxDC") != 0)
    {
        /* Set __index field. */
        lua_pushcfunction(L, dc_get_index);
        lua_setfield(L, -2, "__index");
        /* Set __gc field. */
        lua_pushcfunction(L, dc_gc);
        lua_setfield(L, -2, "__gc");
    }
    lua_pop(L, 1);
    lua_pop(L, 1);
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
    lua_setfield(L, -2, "float_bytes");
    lua_pushcfunction(L, u32bytes);
    lua_setfield(L, -2, "u32_bytes");
    
    #include "class.lua.h"
    #include "graph.lua.h"
    #include "functions.lua.h"
    #include "pickle.lua.h"
    /*if (
        load_lua(L, "C:\\PS3\\Insomniac\\projects\\shader\\src\\class.lua", 0) != 0 ||
        load_lua(L, "C:\\PS3\\Insomniac\\projects\\shader\\src\\graph.lua", 0) != 0 ||
        load_lua(L, "C:\\PS3\\Insomniac\\projects\\shader\\src\\functions.lua", 0) != 0 ||
        load_lua(L, "C:\\PS3\\Insomniac\\projects\\shader\\src\\pickle.lua", 0) != 0
    ) {
        wxMessageDialog dialog(NULL, lua_tostring(L, -1), wxT("Error"), wxOK);
        dialog.ShowModal();
        lua_close(L);
        return NULL;
    }*/
    
    dc_init(L);

    lua_settop(L, 0);
    return L;
}

void
luaB_setfuncs(lua_State *L, wxMenu *edit, wxStatusBar *status, wxPropertyEditor *props, Graph *graph, wxTextCtrl *code, wxNotebook *ph)
{
    lua_newtable(L);
    
    lua_pushlightuserdata(L, (void *)edit);
    lua_pushcclosure(L, menu_ctrl, 1);
    lua_setfield(L, -2, "menu_ctrl");
    
    lua_pushlightuserdata(L, (void *)status);
    lua_pushcclosure(L, set_status, 1);
    lua_setfield(L, -2, "set_status");
    
    lua_pushlightuserdata(L, (void *)graph);
    lua_pushcclosure(L, set_dot_data, 1);
    lua_setfield(L, -2, "set_dot_data");
    
    lua_pushlightuserdata(L, (void *)graph);
    lua_pushcclosure(L, get_xdot_data, 1);
    lua_setfield(L, -2, "get_xdot_data");
    
    lua_pushlightuserdata(L, (void *)graph);
    lua_pushcclosure(L, redraw, 1);
    lua_setfield(L, -2, "graph_redraw");
    
    lua_pushlightuserdata(L, (void *)code);
    lua_pushcclosure(L, set_code, 1);
    lua_setfield(L, -2, "set_code");

    lua_pushlightuserdata(L, (void *)code);
    lua_pushcclosure(L, add_code, 1);
    lua_setfield(L, -2, "add_code");
    
    lua_pushlightuserdata(L, (void *)graph);
    lua_pushcclosure(L, popup, 1);
    lua_setfield(L, -2, "popup");
    
    lua_pushlightuserdata(L, (void *)graph);
    lua_pushcclosure(L, set_tooltip, 1);
    lua_setfield(L, -2, "set_tooltip");
    
    lua_pushlightuserdata(L, (void *)props);
    lua_pushcclosure(L, props_clear, 1);
    lua_setfield(L, -2, "props_clear");

    lua_pushlightuserdata(L, (void *)props);
    lua_pushcclosure(L, props_delete, 1);
    lua_setfield(L, -2, "props_delete");

    lua_pushlightuserdata(L, (void *)props);
    lua_pushcclosure(L, props_redraw, 1);
    lua_setfield(L, -2, "props_redraw");

    lua_pushlightuserdata(L, (void *)props);
    lua_pushcclosure(L, props_text_input, 1);
    lua_setfield(L, -2, "props_text_input");

    lua_pushlightuserdata(L, (void *)props);
    lua_pushcclosure(L, props_memo_input, 1);
    lua_setfield(L, -2, "props_memo_input");

    lua_pushlightuserdata(L, (void *)props);
    lua_pushcclosure(L, props_check_box, 1);
    lua_setfield(L, -2, "props_check_box");

    lua_pushlightuserdata(L, (void *)props);
    lua_pushcclosure(L, props_list, 1);
    lua_setfield(L, -2, "props_list");
    
    lua_pushlightuserdata(L, (void *)graph);
    lua_pushcclosure(L, set_modified, 1);
    lua_setfield(L, -2, "set_modified");
    
    lua_pushlightuserdata(L, (void *)ph);
    lua_pushcclosure(L, set_page, 1);
    lua_setfield(L, -2, "set_page");
    
    lua_setglobal(L, "c_lib");
}
