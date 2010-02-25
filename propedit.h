#ifndef __PROPEDIT_H__
#define __PROPEDIT_H__

#include <wx/wx.h>
#include <wx/hashmap.h>
#include <wx/propgrid/propgrid.h>

class wxPropertyValidator
{
    public:
        virtual void SetValue(wxVariant& value) = 0;
        virtual wxVariant GetValue() = 0;
        virtual bool Validate(wxVariant& value) = 0;
};

WX_DECLARE_HASH_MAP(
    wxPGProperty *,
    wxPropertyValidator *,
    wxPointerHash,
    wxPointerEqual,
    PGId2Validator
);

class wxPropertyEditor: public wxPropertyGrid
{
    public:
        wxPropertyEditor(wxWindow *parent);
        void OnPropertyChange(wxPropertyGridEvent& evt);
        void SetPropertyValidator(const wxString& name, wxPropertyValidator *validator);
        void SetPropertyValidator(wxPGId id, wxPropertyValidator *validator);
    
    private:
        PGId2Validator hash;

    DECLARE_NO_COPY_CLASS(wxPropertyEditor)
    DECLARE_EVENT_TABLE()
};

#endif // __PROPEDIT_H__
