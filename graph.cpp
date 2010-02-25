#include <wx/wx.h>
#include <wx/dir.h>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include "graph.h"
#include "luabind.h"

BEGIN_EVENT_TABLE(Graph, GraphViz)
    EVT_LEFT_UP(Graph::OnLeftUp)
    EVT_MOTION(Graph::OnMotion)
    EVT_KEY_DOWN(Graph::OnKeyDown)
END_EVENT_TABLE()

static inline int
call(wxWindow *parent, lua_State *L, int nargs, int nresults)
{
    int top = lua_gettop(L);
    int res = lua_pcall(L, nargs, nresults, 0);
    if (res != 0)
    {
        wxMessageDialog dialog(parent, wxString::FromAscii(lua_tostring(L, -1)), wxT("Error 2"), wxOK);
        dialog.ShowModal();
        lua_settop(L, top - nargs - 1 + nresults);
        return res;
    }
    return 0;
}

Graph::Graph(wxWindow *parent, const char *dot_path, lua_State *L): GraphViz(parent, dot_path)
{
    this->L = L;
    tooltip = new wxToolTip(wxT(""));
    wxToolTip::SetDelay(2000);
    SetToolTip(tooltip);
    modified = false;
}

void
Graph::Paint(wxDC& dc)
{
    GraphViz::Paint(dc);
    lua_getglobal(L, "finish");
    luaB_pushdc(L, &dc);
    call(this, L, 1, 0);
}

void
Graph::OnLeftUp(wxMouseEvent& evt)
{
    if (!wxCanvas::HasDragged())
    {
        int x, y;
        MouseToCanvas(evt.GetX(), evt.GetY(), &x, &y);
        lua_getglobal(L, "left_click");
        lua_pushnumber(L, x);
        lua_pushnumber(L, y);
        lua_pushboolean(L, evt.ShiftDown());
        lua_pushboolean(L, evt.ControlDown());
        lua_pushboolean(L, evt.AltDown());
        call(this, L, 5, 0);
    }
    evt.Skip();
}

void
Graph::OnMotion(wxMouseEvent& evt)
{
    if (!evt.Dragging())
    {
        int x, y;
        MouseToCanvas(evt.GetX(), evt.GetY(), &x, &y);
        lua_getglobal(L, "motion");
        lua_pushnumber(L, x);
        lua_pushnumber(L, y);
        lua_pushboolean(L, evt.ShiftDown());
        lua_pushboolean(L, evt.ControlDown());
        lua_pushboolean(L, evt.AltDown());
        call(this, L, 5, 0);
    }
    evt.Skip();
}

void
Graph::OnKeyDown(wxKeyEvent& evt)
{
    int key = evt.GetKeyCode();
    if (key == WXK_DELETE)
    {
        lua_getglobal(L, "delete");
        call(this, L, 0, 0);
    }
    else
        evt.Skip();
}

void
Graph::SetLuaState(lua_State *L)
{
    this->L = L;
}

void
Graph::New()
{
    lua_getglobal(L, "new");
    call(this, L, 0, 0);
}

void
Graph::Open(const char *name)
{
    lua_getglobal(L, "open");
    lua_pushstring(L, name);
    call(this, L, 1, 0);
    Redraw(true);
}

void
Graph::Save(const char *name)
{
    lua_getglobal(L, "save");
    lua_pushstring(L, name);
    call(this, L, 1, 0);
}

void
Graph::Undo()
{
    lua_getglobal(L, "undo");
    call(this, L, 0, 0);
}

void
Graph::Redo()
{
    lua_getglobal(L, "redo");
    call(this, L, 0, 0);
}

void
Graph::Cut()
{
    lua_getglobal(L, "cut");
    call(this, L, 0, 0);
}

void
Graph::Copy()
{
    lua_getglobal(L, "copy");
    call(this, L, 0, 0);
}

void
Graph::Paste()
{
    lua_getglobal(L, "paste");
    call(this, L, 0, 0);
}

void
Graph::Duplicate()
{
    lua_getglobal(L, "duplicate");
    call(this, L, 0, 0);
}

void
Graph::AddNode(const char *name_space, const char *name)
{
    lua_getglobal(L, "add_node");
    lua_pushstring(L, name_space);
    lua_pushstring(L, name);
    call(this, L, 2, 0);
}

wxString
Graph::GetHelp(const char *name_space, const char *name)
{
    lua_getglobal(L, "get_help");
    lua_pushstring(L, name_space);
    if (name != NULL)
        lua_pushstring(L, name);
    else
        lua_pushnil(L);
    call(this, L, 2, 1);
    wxString help = wxString::FromAscii(luaL_checkstring(L, -1));
    lua_pop(L, 1);
    return help;
}

bool
Graph::IsModified()
{
    return modified;
}

void
Graph::SetModified(bool modified)
{
    this->modified = modified;
}
