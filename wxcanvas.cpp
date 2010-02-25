#include <wx/metafile.h>
#include <wx/dcbuffer.h>
#include <math.h>
#include "wxcanvas.h"
#include "wxbitmapdc.h"

BEGIN_EVENT_TABLE(wxCanvas, wxScrolledWindow)
    EVT_SIZE(wxCanvas::OnResize)
    EVT_LEFT_DOWN(wxCanvas::OnLeftDown)
    EVT_LEFT_UP(wxCanvas::OnLeftUp)
    EVT_MOTION(wxCanvas::OnMotion)
    EVT_MOUSEWHEEL(wxCanvas::OnMouseWheel)
    EVT_KEY_DOWN(wxCanvas::OnKeyDown)
    EVT_ERASE_BACKGROUND(wxCanvas::OnEraseBackground)
    EVT_PAINT(wxCanvas::OnPaint)
END_EVENT_TABLE()

wxCanvas::wxCanvas(wxWindow* parent) :
    wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxALWAYS_SHOW_SB | wxHSCROLL | wxVSCROLL),
    hand(wxCURSOR_SIZING),
    zoom(1),
    width(0),
    height(0),
    zooming(false)
{
    SetVirtualSize(0, 0);
    SetScrollRate(1, 1);
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

void
wxCanvas::OnResize(wxSizeEvent &evt)
{
    Redraw(false);
    evt.Skip();
}

void
wxCanvas::OnLeftDown(wxMouseEvent &evt)
{
    dragged = false;
    left_x = evt.GetX();
    left_y = evt.GetY();
    evt.Skip();
}

void
wxCanvas::OnLeftUp(wxMouseEvent& evt)
{
    if (dragged)
    {
        dragged = false;
        Redraw(false);
        SetCursor(*wxSTANDARD_CURSOR);
    }
    evt.Skip();
}

void
wxCanvas::OnMotion(wxMouseEvent& evt)
{
    mouse_x = evt.GetX();
    mouse_y = evt.GetY();
    MouseToCanvas(mouse_x, mouse_y, &canvas_x, &canvas_y);
    if (evt.Dragging())
    {
        int dx = left_x - mouse_x;
        int dy = left_y - mouse_y;
        if (dragged || abs(dx) > 5 || abs(dy) > 5)
        {
            if (!dragged)
            {
                dragged = true;
                SetCursor(hand);
            }
            left_x = mouse_x;
            left_y = mouse_y;
            int view_x, view_y;
            GetViewStart(&view_x, &view_y);
            Scroll(view_x + dx, view_y + dy);
        }
    }
    else
    {
        //SetCursor(*wxSTANDARD_CURSOR);
    }
    evt.Skip();
}

void
wxCanvas::OnMouseWheel(wxMouseEvent& evt)
{
    if (evt.GetWheelRotation() > 0)
        SetZoom(zoom * 1.25);
    else
        SetZoom(zoom / 1.25);
}

void
wxCanvas::OnKeyDown(wxKeyEvent& evt)
{
    int x, y;
    
    int key = evt.GetKeyCode();
    switch (key)
    {
        case WXK_PAGEUP:
            SetZoom(zoom * 1.25);
            break;
        case WXK_PAGEDOWN:
            SetZoom(zoom / 1.25);
            break;
        case WXK_HOME:
            SetZoom(1.0);
        case WXK_UP:
            GetViewStart(&x, &y);
            Scroll(x, y - 10);
            break;
        case WXK_DOWN:
            GetViewStart(&x, &y);
            Scroll(x, y + 10);
            break;
        case WXK_LEFT:
            GetViewStart(&x, &y);
            Scroll(x - 10, y);
            break;
        case WXK_RIGHT:
            GetViewStart(&x, &y);
            Scroll(x + 10, y);
            break;
        default:
            evt.Skip();
            break;
    }
}

void
wxCanvas::OnEraseBackground(wxEraseEvent& evt)
{
    (void)evt;
}

void
wxCanvas::OnPaint(wxPaintEvent& evt)
{
    // __REMARK__
    // Prevously we were using a wxMetafileDC to both evaluate the
    // graph size and serve as a cache to draw it to a wxBitmapDC and
    // avoid having to parse the xdot file again. the wxBitmapDC then
    // was used on all paint operations until the user changed the
    // zoom value.
    // __REMARK__
    // Although this code works on both Windows and Linux, it still
    // doesn't work as expected. It seems that wxAutoBufferedPaintDC
    // is not being able to avoid the flicker. The good part is that
    // the code is now much faster than maintaining a wxBitmapDC
    // with the graph, because it takes a lot of memory on high zoom
    // values and time when the bitmap must be resized.
    // __BUG__
    // wxAutoBufferedDC is not helping with the flicker.
    // __TODO__
    // Avoid the flickering.
    wxAutoBufferedPaintDC dc(this);
    DoPrepareDC(dc);
    // Get the size of the client area.
    int cl_width, cl_height;
    GetClientSize(&cl_width, &cl_height);
    // The deltas start at 0.
    int dx = 0, dy = 0;
    // If the width of the drawing is less than the width
    // of the client area, we compute the delta to
    // centralize horizontally. width and height are
    // computed in another place.
    if (width < cl_width)
        dx = (cl_width - width) / 2;
    // The same for the height.
    if (height < cl_height)
        dy = (cl_height - height) / 2;
    // Get the pre-computed offset.
    wxCoord x, y;
    dc.GetDeviceOrigin(&x, &y);
    // Set the user scale.
    dc.SetUserScale(zoom, zoom);
    // Clear the client area.
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    // Set the offset to the original one plus the deltas.
    dc.SetDeviceOrigin(x + dx, y + dy);
    // Paint will draw whatever it wants to draw.
    Paint(dc);
    // Set the user scale back to 1.0, 1.0 to avoid
    // pixalation.
    // __REMARK__
    // Windows doesn't need it but on Linux
    // we get a pixelated image if we don't do it.
    dc.SetUserScale(1.0, 1.0);
}

bool
wxCanvas::HasDragged()
{
    return dragged;
}

void
wxCanvas::Redraw(bool force)
{
    if (force)
    {
        // __REMARK__
        // The redrawing is actually just an evaluation of the
        // graph size so that we can set the virtual size of the
        // wxCanvas accordingly.
        // __TODO__
        // Save the previous values of the mouse position and
        // use them to centralize the graph on it while zooming.
        wxBitmapDC bdc;
        Paint(bdc);
        // Oddly enough, if we directly set width and height
        // the drawing offsets are uncorrectly computed...
        int new_w = (int)ceil((bdc.MaxX() + 1) * zoom);
        int new_h = (int)ceil((bdc.MaxY() + 1) * zoom);
        SetVirtualSize(new_w, new_h);
        if (zooming)
        {
            // Find what the previous coordinates of the mouse
            // would be with the new zoom.
            int previous_x, previous_y;
            CanvasToMouse(canvas_x, canvas_y, &previous_x, &previous_y);
            // Get the view start...
            int x, y;
            GetViewStart(&x, &y);
            // and move it to centralize the drawing in the mouse.
            Scroll(x + previous_x - mouse_x, y + previous_y - mouse_y);
            // Update the canvas position.
            MouseToCanvas(mouse_x, mouse_y, &canvas_x, &canvas_y);
        }
        width = new_w;
        height = new_h;
    }
    Refresh(false);
}

double
wxCanvas::SetZoom(double zoom)
{
    if (zoom > 0.1 && zoom < 4)
    {
        double old = this->zoom;
        this->zoom = zoom;
        // Set the zooming flag to disable ScrollWindow.
        zooming = true;
        Redraw(true);
        // Reset it for normal operation.
        zooming = false;
        return old;
    }
    return this->zoom;
}

double
wxCanvas::GetZoom()
{
    return zoom;
}

bool
wxCanvas::CopyAsMetafile()
{
#ifdef __WIN32__
    wxMetafileDC mdc;
    Paint(mdc);
    wxMetafile *meta = mdc.Close();
    bool ok = meta->SetClipboard();
    delete meta;
    return ok;
#else
    return false;
#endif
}

void
wxCanvas::ScrollWindow(int dx, int dy, const wxRect *rect)
{
    // If we're zooming, we disable the physical copy of the
    // pixels and let OnPaint redraw the client area.
    if (!zooming)
        wxScrolledWindow::ScrollWindow(dx, dy, rect);
}

void
wxCanvas::MouseToCanvas(int mouse_x, int mouse_y, int *bm_x, int *bm_y)
{
    int cl_width, cl_height;
    GetClientSize(&cl_width, &cl_height);
    int dx = 0, dy = 0;
    if (width < cl_width)
        dx = (cl_width - width) / 2;
    if (height < cl_height)
        dy = (cl_height - height) / 2;
    wxClientDC dc(this);
    DoPrepareDC(dc);
    dc.SetUserScale(zoom, zoom);
    *bm_x = dc.DeviceToLogicalX(mouse_x - dx);
    *bm_y = dc.DeviceToLogicalY(mouse_y - dy);
}

void
wxCanvas::CanvasToMouse(int bm_x, int bm_y, int *mouse_x, int *mouse_y)
{
    int cl_width, cl_height;
    GetClientSize(&cl_width, &cl_height);
    int dx = 0, dy = 0;
    if (width < cl_width)
        dx = (cl_width - width) / 2;
    if (height < cl_height)
        dy = (cl_height - height) / 2;
    wxClientDC dc(this);
    DoPrepareDC(dc);
    dc.SetUserScale(zoom, zoom);
    *mouse_x = dc.LogicalToDeviceX(bm_x) + dx;
    *mouse_y = dc.LogicalToDeviceY(bm_y) + dy;
}
