#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>
#include "propedit.h"

BEGIN_EVENT_TABLE(wxPropertyEditor, wxPropertyGrid)
    EVT_PG_CHANGED(wxID_ANY, wxPropertyEditor::OnPropertyChange)
END_EVENT_TABLE()

wxPropertyEditor::wxPropertyEditor(wxWindow *parent): wxPropertyGrid(parent)
{
}

void
wxPropertyEditor::OnPropertyChange(wxPropertyGridEvent& evt)
{
    wxPGId id = evt.GetProperty();
    wxPropertyValidator *validator = hash[id.GetPropertyPtr()];
    wxVariant value = GetPropertyValue(id);
    if (validator != NULL)
        if(validator->Validate(value))
            validator->SetValue(value);
        else
        {
            value = validator->GetValue();
            SetPropertyValue(id, value);
        }
}

void
wxPropertyEditor::SetPropertyValidator(const wxString& name, wxPropertyValidator *validator)
{
    wxPGId id = GetPropertyByName(name);
    SetPropertyValidator(id, validator);
}

void
wxPropertyEditor::SetPropertyValidator(wxPGId id, wxPropertyValidator *validator)
{
    hash[id.GetPropertyPtr()] = validator;
    wxVariant value = validator->GetValue();
    SetPropertyValue(id, value);
}
