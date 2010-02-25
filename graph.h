#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <wx/wx.h>
#include "propedit.h"
#include "graphviz.h"

extern "C" {
    #include "lua.h"
};

class Graph: public GraphViz
{
    public:
        Graph(wxWindow *parent, const char *dot_path, lua_State *L);
        virtual void Paint(wxDC& dc);
    
        void OnLeftUp(wxMouseEvent& evt);
        void OnMotion(wxMouseEvent& evt);
        void OnKeyDown(wxKeyEvent& evt);

        void SetLuaState(lua_State *L);
        void New();
        void Open(const char *name);
        void Save(const char *name);
        void Undo();
        void Redo();
        void Cut();
        void Copy();
        void Paste();
        void Duplicate();
        void AddNode(const char *name_space, const char *name);
        wxString GetHelp(const char *name_space, const char *name);
        bool IsModified();
        void SetModified(bool modified);

    private:
        lua_State *L;
        wxToolTip *tooltip;
        bool modified;

    DECLARE_NO_COPY_CLASS(Graph)
    DECLARE_EVENT_TABLE()
};

#endif /* __GRAPH_H__ */
