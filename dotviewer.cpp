#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "wxcanvas.h"
#include "graphviz.h"

class MainFrame: public wxFrame
{
    public:
        MainFrame();
        ~MainFrame() { delete canvas; }
        void OnQuit(wxCommandEvent& evt);
        void OnCommand(wxCommandEvent& evt);
    private:
        GraphViz *canvas;
    
    DECLARE_NO_COPY_CLASS(MainFrame)
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnQuit)
    EVT_MENU(wxID_ANY, MainFrame::OnCommand)
END_EVENT_TABLE()

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, wxT("Dot Viewer"))
{
    wxMenuBar *menu = new wxMenuBar(0);

    wxMenu *file = new wxMenu();
    file->Append(wxID_OPEN);
    file->Append(wxID_EXIT);
    menu->Append(file, wxT("&File"));

    this->SetMenuBar(menu);
    
#ifdef __WIN32__
    char dot_path[MAX_PATH];
    GetModuleFileName(NULL, dot_path + 1, sizeof(dot_path) - 1);
    dot_path[0] = '"';
    *(strrchr(dot_path, '\\') + 1) = '\0';
    strcat(dot_path, "dot\\dot.exe\"");
    canvas = new GraphViz(this, dot_path);
#else
    canvas = new GraphViz(this, "dot");
#endif
}

void
MainFrame::OnQuit(wxCommandEvent& evt)
{
    Close(true);
}

void
MainFrame::OnCommand(wxCommandEvent& evt)
{
    if (evt.GetId() == wxID_OPEN)
    {
        wxString name = ::wxFileSelector(wxT("Open file"), wxT(""), wxT(""), wxT("dot"), wxT("Dot files (*.dot)|*.dot"), wxOPEN | wxFILE_MUST_EXIST);
        if (!name.empty()) {
            canvas->Open((const char *)name.fn_str());
            canvas->Refresh(false);
        }
    }
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
    MainFrame *frame = new MainFrame();
    frame->Show(true);
    return true;
}
