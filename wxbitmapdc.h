#ifndef __WXBUFDC_H__
#define __WXBUFDC_H__

#include <wx/wx.h>

class wxBitmapDC: public wxMemoryDC
{
    public:
        wxBitmapDC();
        ~wxBitmapDC();
        void SetBitmapSize(int width, int height);
        int GetBitmapWidth();
        int GetBitmapHeight();
        const wxBitmap *GetBitmap();
    
        /*void DrawPolygon(int n, wxPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0, int fill_style = wxODDEVEN_RULE);
        void DrawSpline(int n, wxPoint points[]);
        void DrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2);
        void DrawText(const wxString& text, wxCoord x, wxCoord y);
        void DrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height);*/

    private:
        int bm_width, bm_height;
        wxBitmap *bitmap;
};

#endif
