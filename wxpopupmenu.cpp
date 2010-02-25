#include "wxpopupmenu.h"

BEGIN_EVENT_TABLE(wxPopupMenu, wxMenu)
    EVT_MENU(wxID_ANY, wxPopupMenu::OnItemSelected)
END_EVENT_TABLE()

wxPopupMenu::wxPopupMenu() :
    wxMenu(0), item_count(0), selected(-1)
{
}

int
wxPopupMenu::GetNextId()
{
    return wxID_HIGHEST + item_count++;
}

void
wxPopupMenu::OnItemSelected(wxCommandEvent& evt)
{
    selected = evt.GetId() - wxID_HIGHEST;
}

int
wxPopupMenu::Popup(wxWindow *parent)
{
    selected = -1;
    if (item_count != 0)
        parent->PopupMenu(this);
    return selected;
}
