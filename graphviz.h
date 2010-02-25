
#ifndef __GRAPHVIZ_H__
#define __GRAPHVIZ_H__

#include <wx/wx.h>
#include "wxcanvas.h"
#include "xdot.h"

class GraphViz: public wxCanvas
{
    public:
        GraphViz(wxWindow *parent, const char *dot_path);
        ~GraphViz();

        virtual void Paint(wxDC& dc);
    
        void Open(const char *name);
        void SetDotData(const char *dot_data);
        const char *GetXDotData();

    private:
        char *dot_path, *dot_data, *xdot_data;
        bool dot_changed;
        XDot xdot;
};

#endif // __GRAPHVIZ_H__
