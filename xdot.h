#ifndef __XDOT_H__
#define __XDOT_H__

#include <vector>
#include <wx/dc.h>

class XDotCommand
{
    public:
        virtual ~XDotCommand() {};
        virtual void Execute(wxDC& dc) = 0;
};

class XDot: public XDotCommand
{
    public:
        XDot();
        virtual ~XDot();
    
        void Clear();
        
        void AddSetPen(unsigned int rgba, int width, int style, bool visible);
        void AddSetFillColor(unsigned int rgba);
        void AddFilledPolygon(int count, wxPoint *points);
        void AddBSpline(int count, wxPoint *points);
        void AddPolygon(int count, wxPoint *points);
        void AddPolyline(int count, wxPoint *points);
        void AddSetFont(int family, int style, int weight, int size);
        void AddText(int x, int y, int alignment, const char *text);
        void AddFilledEllipse(int x, int y, int width, int height);
        void AddEllipse(int x, int y, int width, int height);
            
        virtual void Execute(wxDC& dc);

        void Parse(const char *xdot);

    private:
        void ParseDrawingCmds(const char *cmds, const char *end);
    
        std::vector<XDotCommand *> commands;
};

#endif // __GRAPHVIZ_H__
