#ifndef __WXPOPUPMENU_H__
#define __WXPOPUPMENU_H__

#include <wx/wx.h>

class wxPopupMenu: public wxMenu
{
    public:
        wxPopupMenu();
    
        int GetNextId();
        void OnItemSelected(wxCommandEvent& evt);
        int Popup(wxWindow *parent);
    
    private:
        int item_count, selected;

    DECLARE_NO_COPY_CLASS(wxPopupMenu)
    DECLARE_EVENT_TABLE()
};

#endif
