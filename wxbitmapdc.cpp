#include "wxbitmapdc.h"

wxBitmapDC::wxBitmapDC() :
    wxMemoryDC(), bitmap(NULL), bm_width(-1), bm_height(-1)
{
    // __REMARK__
    // On Linux, wxMemoryDC only computes the
    // bouding box of the drawing if we attach a
    // wxBitmap to it, no matter what size.
    bitmap = new wxBitmap(1, 1);
    SelectObject(*bitmap);
}

wxBitmapDC::~wxBitmapDC()
{
    SelectObject(wxNullBitmap);
    delete bitmap;
}

void
wxBitmapDC::SetBitmapSize(int width, int height)
{
    if (width > bitmap->GetWidth() || height > bitmap->GetHeight()) // || width < bitmap->GetWidth() / 2 || height < bitmap->GetWidth() / 2)
    {
        SelectObject(wxNullBitmap);
        delete bitmap;
        bitmap = new wxBitmap(width, height);
        SelectObject(*bitmap);
    }
    bm_width = width;
    bm_height = height;
}

int
wxBitmapDC::GetBitmapWidth()
{
    return bm_width;
}

int
wxBitmapDC::GetBitmapHeight()
{
    return bm_height;
}

const wxBitmap *
wxBitmapDC::GetBitmap()
{
    return bitmap;
}
