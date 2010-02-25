#ifndef __WXCANVAS_H__
#define __WXCANVAS_H__

#include <wx/wx.h>
#include "wxbitmapdc.h"

class wxCanvas: public wxScrolledWindow
{
    public:
        wxCanvas(wxWindow *parent);

        virtual void Paint(wxDC& dc) = 0;

        void OnResize(wxSizeEvent& evt);
        void OnLeftDown(wxMouseEvent& evt);
        void OnLeftUp(wxMouseEvent& evt);
        void OnMotion(wxMouseEvent& evt);
        void OnMouseWheel(wxMouseEvent& evt);
        void OnMouseLost(wxMouseCaptureLostEvent& evt);
        void OnKeyDown(wxKeyEvent& evt);
        void OnEraseBackground(wxEraseEvent& evt);
        void OnPaint(wxPaintEvent& evt);
        //virtual void OnDraw(wxDC& dc);

        bool HasDragged();
        void Redraw(bool force = false);
        double SetZoom(double factor);
        double GetZoom();
            
        bool CopyAsMetafile();

        virtual void ScrollWindow(int dx, int dy, const wxRect *rect = NULL);

    protected:
        void MouseToCanvas(int mouse_x, int mouse_y, int *bm_x, int *bm_y);
        void CanvasToMouse(int bm_x, int bm_y, int *mouse_x, int *mouse_y);

    private:
        wxCursor hand;
        double zoom;
        bool dragged;
        int mouse_x, mouse_y, canvas_x, canvas_y, left_x, left_y;
        wxBitmapDC bitmap;
        int width, height;
        bool zooming;
    
    DECLARE_NO_COPY_CLASS(wxCanvas)
    DECLARE_EVENT_TABLE()
};

#endif // __WXCANVAS_H__
