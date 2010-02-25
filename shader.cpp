#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/propgrid/propgrid.h>
#include <wx/dir.h>
#include <wx/filedlg.h>
#include <wx/notebook.h>
#include <wx/filename.h>
#include <wx/config.h>
#include <errno.h>

#include "wxcanvas.h"
#include "wxpopupmenu.h"
#include "graph.h"
#include "propedit.h"
#include "luabind.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
};

#define wxID_COPYMETAFILE    (wxID_HIGHEST + 0)
#define wxID_COPYCODE    (wxID_HIGHEST + 1)
#define wxID_RELOAD        (wxID_HIGHEST + 2)
#define wxID_SAVECODE    (wxID_HIGHEST + 3)

class MainFrame: public wxFrame
{
    public:
        MainFrame();
        ~MainFrame() { lua_close(L); }
        void OnClose(wxCloseEvent& evt);
        void OnQuit(wxCommandEvent& evt);
        void OnCommand(wxCommandEvent& evt);
        void OnItemActivated(wxTreeEvent& evt);
        void OnSelChanged(wxTreeEvent& evt);
        wxString file_path, code_path;

    private:
        int GetItemLevel(wxTreeItemId item);
        void SaveConfig();
        void ReloadLibs();
        void OpenLibs();
        void Open();
        void Save();
        void SaveAs();
        void SaveCode();
        void ConfirmSave();
        wxMenu *edit;
        wxStatusBar *status;
        wxPropertyEditor *props;
        Graph *graph;
        wxTextCtrl *code, *comments;
        wxTreeCtrl *nodes;
        wxNotebook *ph;
        lua_State *L;
        wxString file_name, code_name;
        wxSplitterWindow *versplitter, *horsplitter;
    
    DECLARE_NO_COPY_CLASS(MainFrame)
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_CLOSE(MainFrame::OnClose)
    EVT_MENU(wxID_EXIT, MainFrame::OnQuit)
    EVT_MENU(wxID_ANY, MainFrame::OnCommand)
    EVT_TREE_ITEM_ACTIVATED(wxID_ANY, MainFrame::OnItemActivated)
    EVT_TREE_SEL_CHANGED(wxID_ANY, MainFrame::OnSelChanged) 
END_EVENT_TABLE()

typedef struct
{
    int functions;
} regdata_t;

static int include(lua_State *L)
{
    const char *lib_path = (const char *)lua_tostring(L, lua_upvalueindex(1));
    const char *file_name = lua_tostring(L, 1);
    
    wxString path = wxString::FromAscii(lib_path);
    path.Append(file_name);
    if (load_lua(L, (const char *)path.c_str(), 0) != 0)
    {
        printf("%s\n", lua_tostring(L, -1)); fflush(stdout);
        lua_error(L);
        return 0;
    }
    return 0;
}

static int lregister(lua_State *L)
{
    regdata_t *rd = (regdata_t *)lua_touserdata(L, lua_upvalueindex(1)); // table
    if (rd->functions == LUA_NOREF)
    {
        lua_newtable(L); // table, master
        rd->functions = luaL_ref(L, LUA_REGISTRYINDEX); // table
    }
    lua_rawgeti(L, LUA_REGISTRYINDEX, rd->functions); // table, master
    lua_pushnil(L);  // table, master, nil
    while (lua_next(L, 1) != 0) // table, master, key, value
    {
        lua_pushvalue(L, 3); // table, master, key, value, key
        lua_gettable(L, 2); // table, master, key, value, value
        if (lua_isnil(L, 5))
        {
            lua_pop(L, 1); // table, master, key, value
            lua_pushvalue(L, 3); // table, master, key, value, key
            lua_insert(L, 4); // table, master, key, key, value
            lua_settable(L, 2); // table, master, key
        }
        else
            lua_pop(L, 2); // table, master, key
    }
    lua_pop(L, 1);
    return 0;
}

void
MainFrame::OpenLibs()
{
    wxTreeItemId root = nodes->AddRoot(wxT("Nodes"));
#ifdef __WIN32__
    char lib_path[MAX_PATH];
    GetModuleFileName(NULL, lib_path, sizeof(lib_path));
    *(strrchr(lib_path, '\\') + 1) = '\0';
    strcat(lib_path, "lib\\");
    wxDir dir(lib_path);
#else
    const char *lib_path = "/usr/local/shader/lib/";
    wxDir dir(wxString::FromAscii(lib_path));
#endif
    if (!dir.IsOpened())
        return;
    regdata_t rd;
    lua_pushstring(L, lib_path);
    lua_pushcclosure(L, include, 1);
    lua_setglobal(L, "include");
    lua_pushlightuserdata(L, (void *)&rd);
    lua_pushcclosure(L, lregister, 1);
    lua_setglobal(L, "register");
    wxString filename;
    bool cont = dir.GetFirst(&filename, wxT("*.nodes"), wxDIR_FILES);
    while (cont)
    {
        rd.functions = LUA_NOREF;
        filename.Prepend(wxString::FromAscii(lib_path));
        if (load_lua(L, (const char *)filename.c_str(), 0) != 0)
        {
            wxMessageDialog dialog(this, wxString::FromAscii(lua_tostring(L, -1)), wxT("Error"), wxOK);
            dialog.ShowModal();
            return;
        }
        if (rd.functions != LUA_NOREF)
        {
            lua_rawgeti(L, LUA_REGISTRYINDEX, rd.functions);
            lua_getfield(L, -1, "__name");
            wxString name_space = wxString::FromAscii(lua_tostring(L, -1));
            lua_pop(L, 1);
            wxTreeItemId library = nodes->AppendItem(root, name_space);
            lua_pushvalue(L, -1);
            lua_setglobal(L, name_space.c_str());
            lua_pushnil(L);  // first key
            while (lua_next(L, -2) != 0)
            {
                lua_pushvalue(L, -2);
                const char *name = lua_tostring(L, -1);
                if (name[0] != '_' || name[1] != '_')
                    nodes->AppendItem(library, wxString::FromAscii(name));
                lua_pop(L, 2);
            }
            lua_pop(L, 1);
            nodes->SortChildren(library);
        }
        cont = dir.GetNext(&filename);
    }
    nodes->SortChildren(root);
    nodes->Expand(root);
    lua_pushnil(L);
    lua_setglobal(L, "register");
    lua_pushnil(L);
    lua_setglobal(L, "include");
}

void
MainFrame::Open()
{
    ConfirmSave();
    wxFileDialog dialog(this, wxT("Open file"), file_path, wxT(""), wxT("Shader files (*.shader)|*.shader|All files (*.*)|*.*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (dialog.ShowModal() == wxID_OK)
    {
        file_name = dialog.GetPath();
        wxFileName fn(file_name);
        file_path = fn.GetPath();
        graph->Open((const char *)file_name.c_str());
        SetTitle(wxT("Shader - ") + wxFileName::FileName(file_name).GetFullName());
    }
}

void
MainFrame::Save()
{
    if (file_name.IsEmpty())
    {
        SaveAs();
        return;
    }
    graph->Save((const char *)file_name.c_str());
}

void
MainFrame::SaveAs()
{
    wxFileDialog dialog(this, wxT("Save as"), file_path, file_name, wxT("Shader files (*.shader)|*.shader"), wxFD_SAVE | wxFD_CHANGE_DIR);
    if (dialog.ShowModal() == wxID_OK)
    {
        file_name = dialog.GetPath();
        wxFileName fn(file_name);
        file_path = fn.GetPath();
        graph->Save((const char *)file_name.c_str());
        SetTitle(wxT("Shader - ") + wxFileName::FileName(file_name).GetFullName());
    }
}

void
MainFrame::SaveCode()
{
    wxFileDialog dialog(this, wxT("Save code"), code_path, code_name, wxT("All files (*.*)|*.*"), wxFD_SAVE | wxFD_CHANGE_DIR);
    if (dialog.ShowModal() == wxID_OK)
        {
        long from, to;
        code->GetSelection(&from, &to);
        code->SetSelection(-1, -1);
        wxString source = code->GetStringSelection();
        code->SetSelection(from, to);
        code_name = dialog.GetPath();
        wxFileName fn(code_name);
        code_path = fn.GetPath();

        FILE* file = fopen( code_name.c_str(), "wb" );
        if (file != NULL)
        {
            fwrite(source.c_str(), 1, source.length(), file);
            fclose(file);
        }
        else
        {
            wxMessageDialog dialog(this, wxString::FromAscii(strerror(errno)), wxT("Error"), wxOK);
            dialog.ShowModal();
        }
    }
}

void
MainFrame::ConfirmSave()
{
    if (graph->IsModified())
    {
        wxString message = wxT("Save changes to ");
        if (file_name.IsEmpty())
            message.Append(wxT("(untitled)"));
        else
            message.Append(file_name);
        message.Append('?');
        wxMessageDialog dialog(this, message, wxT("Confirm"), wxYES_NO);
        if (dialog.ShowModal() == wxID_YES)
            Save();
    }
}

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, wxT("igFragmentShader"))
{
    L = luaB_openlua();

    wxMenuBar *menu = new wxMenuBar(0);

    wxMenu *file = new wxMenu();
    file->Append(wxID_NEW);
    file->Append(wxID_OPEN);
    file->Append(wxID_SAVE);
    file->Append(wxID_SAVEAS, wxT("Save As...\tCtrl-A"));
    file->AppendSeparator();
    file->Append(wxID_SAVECODE, wxT("Save code\tCtrl-G"));
    file->AppendSeparator();
    file->Append(wxID_EXIT, wxT("Quit\tCtrl-Q"));
    menu->Append(file, wxT("&File"));
    
    edit = new wxMenu();
    edit->Append(wxID_UNDO, wxT("&Undo\tCtrl-Z"));
    edit->Append(wxID_REDO, wxT("&Redo\tCtrl-Y"));
    edit->AppendSeparator();
    edit->Append(wxID_CUT);
    edit->Append(wxID_COPY);
    edit->Append(wxID_PASTE);
    edit->Append(wxID_DUPLICATE, wxT("&Duplicate\tCtrl-D"));
    edit->AppendSeparator();
    edit->Append(wxID_COPYMETAFILE, wxT("Copy as &metafile\tCtrl-M"));
    edit->Append(wxID_COPYCODE, wxT("Copy code\tCtrl-P"));
    menu->Append(edit, wxT("&Edit"));
    
    wxMenu *tools = new wxMenu();
    tools->Append(wxID_RELOAD, wxT("Reload libraries\tCtrl-R"));
    menu->Append(tools, wxT("&Tools"));

    this->SetMenuBar(menu);
    
    status = new wxStatusBar(this);
    const int widths[] = { -1, -1, -1 };
    status->SetFieldsCount(3, widths);
    this->SetStatusBar(status);

    versplitter = new wxSplitterWindow(this, wxID_ANY);
    horsplitter = new wxSplitterWindow(versplitter, wxID_ANY);
    
    // top-left window
    nodes = new wxTreeCtrl(horsplitter, wxID_ANY); //, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS | wxTR_HIDE_ROOT);
    
    // bottom-left window
    ph = new wxNotebook(horsplitter, wxID_ANY);
    props = new wxPropertyEditor(ph);
    ph->AddPage(props, wxT("Properties"), true);
    comments = new wxTextCtrl(ph, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);
    ph->AddPage(comments, wxT("Help"));

    // right window
    wxNotebook *notebook = new wxNotebook(versplitter, wxID_ANY);
#ifdef __WIN32__
    char dot_path[MAX_PATH];
    GetModuleFileName(NULL, dot_path + 1, sizeof(dot_path) - 1);
    dot_path[0] = '"';
    *(strrchr(dot_path, '\\') + 1) = '\0';
    strcat(dot_path, "dot\\dot.exe\"");
    graph = new Graph(notebook, dot_path, L);
#else
    graph = new Graph(notebook, "dot", L);
#endif
    notebook->AddPage(graph, wxT("Graph"), true);
    code = new wxTextCtrl(notebook, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);
    notebook->AddPage(code, wxT("Code"));
    
    horsplitter->SplitHorizontally(nodes, ph);
    versplitter->SplitVertically(horsplitter, notebook);
    versplitter->SetMinimumPaneSize(20);
    horsplitter->SetMinimumPaneSize(20);
    luaB_setfuncs(L, edit, status, props, graph, code, ph);

    OpenLibs();
    
    graph->New();
    
    wxConfig *config = new wxConfig(wxT("igFragmentShader"));
    wxRect pos = GetScreenRect();
    long value;
    bool ok = config->Read(wxT("x"), &value);
    pos.SetX(value);
    ok = ok && config->Read(wxT("y"), &value);
    pos.SetY(value);
    ok = ok && config->Read(wxT("width"), &value);
    pos.SetWidth(value);
    ok = ok && config->Read(wxT("height"), &value);
    pos.SetHeight(value);
    ok = ok && config->Read(wxT("filepath"), &file_path);
    ok = ok && config->Read(wxT("codepath"), &code_path);
    long horpos, verpos;
    ok = ok && config->Read(wxT("horsash"), &horpos);
    ok = ok && config->Read(wxT("versash"), &verpos);
    ok = ok && config->Read(wxT("maximized"), &value);
    if (ok)
    {
        SetSize(pos);
        horsplitter->SetSashPosition(horpos, true);
        versplitter->SetSashPosition(verpos, true);
        if (value != 0)
            Maximize();
    }
    delete config;
}

void
MainFrame::ReloadLibs()
{
    graph->Save("C:\\temp.shader");
    graph->New();
    props->Clear();
    lua_close(L);
    L = luaB_openlua();
    luaB_setfuncs(L, edit, status, props, graph, code, ph);
    graph->SetLuaState(L);
    nodes->DeleteAllItems();
    OpenLibs();
    graph->Open("C:\\temp.shader");
    graph->Redraw();
}

void
MainFrame::SaveConfig()
{
    wxConfig *config = new wxConfig(wxT("igFragmentShader"));
    wxRect pos = GetRect();
    config->Write(wxT("x"), pos.GetX());
    config->Write(wxT("y"), pos.GetY());
    config->Write(wxT("width"), pos.GetWidth());
    config->Write(wxT("height"), pos.GetHeight());
    config->Write(wxT("filepath"), file_path);
    config->Write(wxT("codepath"), code_path);
    config->Write(wxT("horsash"), horsplitter->GetSashPosition());
    config->Write(wxT("versash"), versplitter->GetSashPosition());
    config->Write(wxT("maximized"), IsMaximized() ? 1 : 0);
    delete config;
}

void
MainFrame::OnClose(wxCloseEvent& evt)
{
    SaveConfig();
    ConfirmSave();
    Destroy();
}

void
MainFrame::OnQuit(wxCommandEvent& evt)
{
    SaveConfig();
    ConfirmSave();
    Close(true);
}

void
MainFrame::OnCommand(wxCommandEvent& evt)
{
    switch (evt.GetId())
    {
        case wxID_OPEN:
            Open();
            break;
        case wxID_SAVE:
            Save();
            break;
        case wxID_SAVEAS:
            SaveAs();
            break;
        case wxID_SAVECODE:
            SaveCode();
            break;
        case wxID_NEW:
            ConfirmSave();
            graph->New();
            file_name.Empty();
            SetTitle(wxT("Shader"));
            break;
        case wxID_UNDO:
            graph->Undo();
            break;
        case wxID_REDO:
            graph->Redo();
            break;
        case wxID_CUT:
            graph->Cut();
            break;
        case wxID_COPY:
            graph->Copy();
            break;
        case wxID_PASTE:
            graph->Paste();
            break;
        case wxID_DUPLICATE:
            graph->Duplicate();
            break;
        case wxID_COPYMETAFILE:
            graph->CopyAsMetafile();
            break;
        case wxID_COPYCODE:
        {
            long from, to;
            code->GetSelection(&from, &to);
            code->SetSelection(-1, -1);
            code->Copy();
            code->SetSelection(from, to);
            break;
        }
        case wxID_RELOAD:
            ReloadLibs();
            break;
    }
}

int
MainFrame::GetItemLevel(wxTreeItemId item)
{
    wxTreeItemId root = nodes->GetRootItem();
    int level = 0;
    while (item != root)
    {
        level++;
        item = nodes->GetItemParent(item);
    }
    return level;
}

void
MainFrame::OnItemActivated(wxTreeEvent& evt)
{
    wxTreeItemId item = evt.GetItem();
    if (GetItemLevel(item) == 2)
    {
        wxTreeItemId parent = nodes->GetItemParent(item);
        graph->AddNode((const char *)nodes->GetItemText(parent).c_str(), (const char *)nodes->GetItemText(item).c_str());
    }
}

void
MainFrame::OnSelChanged(wxTreeEvent& evt)
{
    wxTreeItemId parent, item = evt.GetItem();
    int level = GetItemLevel(item);
    wxString help;
    switch (level)
    {
        case 1:
            help = graph->GetHelp((const char *)nodes->GetItemText(item).c_str(), NULL).c_str();
            break;
        case 2:
            parent = nodes->GetItemParent(item);
            help = graph->GetHelp((const char *)nodes->GetItemText(parent).c_str(), (const char *)nodes->GetItemText(item).c_str()).c_str();
            break;
        default:
            help = wxString::FromAscii("Click on a item to see a help text about it; double-click it to add the item to the graph");
            break;
    }
    comments->ChangeValue(help);
}

class MyApp: public wxApp
{
    public:
        MyApp() { }
        virtual bool OnInit();

    DECLARE_NO_COPY_CLASS(MyApp)
};

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    MainFrame* frame = new MainFrame();
    frame->Show(true);
    return true;
}
