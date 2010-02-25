#include "xdot.h"

class SetPen: public XDotCommand
{
    public:
        SetPen(unsigned int rgba, int width, int style, bool visible)
        {
            if (visible)
            {
                wxColour color((rgba >> 24) & 255, (rgba >> 16) & 255, (rgba >> 8) & 255, rgba & 255);
                pen = new wxPen(color, width, style);
            }
            else
                pen = wxTRANSPARENT_PEN;
        }
        
        virtual ~SetPen()
        {
            if (pen != wxTRANSPARENT_PEN)
                delete pen;
        }
        
        virtual void Execute(wxDC& dc)
        {
            dc.SetPen(*pen);
        }
        
    private:
        const wxPen *pen;
};

class SetFillColor: public XDotCommand
{
    public:
        SetFillColor(unsigned int rgba)
        {
            wxColour color((rgba >> 24) & 255, (rgba >> 16) & 255, (rgba >> 8) & 255, rgba & 255);
            brush = new wxBrush(color, wxSOLID);
        }
        
        virtual ~SetFillColor()
        {
            delete brush;
        }
        
        virtual void Execute(wxDC& dc)
        {
            dc.SetBrush(*brush);
        }
        
    private:
        wxBrush *brush;
};

class FilledPolygon: public XDotCommand
{
    public:
        FilledPolygon(int count, wxPoint *points)
        {
            this->count = count;
            this->points = points;
        }
        
        virtual ~FilledPolygon()
        {
            delete points;
        }
        
        virtual void Execute(wxDC& dc)
        {
            dc.DrawPolygon(count, points);
        }
        
    private:
        int count;
        wxPoint *points;
};

class BSpline: public XDotCommand
{
    public:
        BSpline(int count, wxPoint *points)
        {
            this->count = count;
            this->points = points;
        }
        
        virtual ~BSpline()
        {
            delete points;
        }
        
        virtual void Execute(wxDC& dc)
        {
            dc.DrawSpline(count, points);
        }
        
    private:
        int count;
        wxPoint *points;
};

class Polygon: public XDotCommand
{
    public:
        Polygon(int count, wxPoint *points)
        {
            this->count = count;
            this->points = points;
        }
        
        virtual ~Polygon()
        {
            delete points;
        }
        
        virtual void Execute(wxDC& dc)
        {
            for (int index = 1; index < count; index++)
                dc.DrawLine(points[index - 1].x, points[index - 1].y, points[index].x, points[index].y);
            dc.DrawLine(points[count - 1].x, points[count - 1].y, points[0].x, points[0].y);
        }
        
    private:
        int count;
        wxPoint *points;
};

class Polyline: public XDotCommand
{
    public:
        Polyline(int count, wxPoint *points)
        {
            this->count = count;
            this->points = points;
        }
        
        virtual ~Polyline()
        {
            delete points;
        }
        
        virtual void Execute(wxDC& dc)
        {
            for (int index = 1; index < count; index++)
                dc.DrawLine(points[index - 1].x, points[index - 1].y, points[index].x, points[index].y);
        }
        
    private:
        int count;
        wxPoint *points;
};

class SetFont: public XDotCommand
{
    public:
        SetFont(int family, int style, int weight, int size)
        {
            font = new wxFont(size, family, style, weight);
        }
        
        virtual ~SetFont()
        {
            delete font;
        }
        
        virtual void Execute(wxDC& dc)
        {
            dc.SetFont(*font);
        }
        
    private:
        wxFont *font;
};

class Text: public XDotCommand
{
    public:
        Text(int x, int y, int alignment, const char *text)
        {
            this->x = x;
            this->y = y;
            this->alignment = alignment;
            init = true;
            string = wxString::FromAscii(text);
            delete text;
        }
        
        virtual ~Text()
        {
        }
        
        virtual void Execute(wxDC& dc)
        {
            if (init)
            {
                init = false;
                dc.GetTextExtent(string, &width, &height, &descent);
                if (alignment == 0)
                    x -= width / 2;
                else if (alignment > 0)
                    x -= width;
                y = y - height + descent;
            }
            dc.DrawText(string, x, y);
        }
        
    private:
        int x, y, alignment, width, height, descent;
        bool init;
        wxString string;
};

class FilledEllipse: public XDotCommand
{
    public:
        FilledEllipse(int x, int y, int width, int height)
        {
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
        }
        
        virtual ~FilledEllipse()
        {
        }
        
        virtual void Execute(wxDC& dc)
        {
            dc.DrawEllipse(x, y, width, height);
        }
        
    private:
        int x, y, width, height;
};

class Ellipse: public XDotCommand
{
    public:
        Ellipse(int x, int y, int width, int height)
        {
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
        }
        
        virtual ~Ellipse()
        {
        }
        
        virtual void Execute(wxDC& dc)
        {
            wxBrush brush = dc.GetBrush();
            dc.SetBrush(*wxTRANSPARENT_BRUSH);
            dc.DrawEllipse(x, y, width, height);
            dc.SetBrush(brush);
        }
        
    private:
        int x, y, width, height;
};

XDot::XDot()
{
}

XDot::~XDot()
{
    Clear();
}

void XDot::Clear()
{
    int length = commands.size();
    for (int index = 0; index < length; index++)
        delete commands[index];
    commands.clear();
}

void XDot::AddSetPen(unsigned int rgba, int width, int style, bool visible)
{
    commands.push_back(new SetPen(rgba, width, style, visible));
}

void XDot::AddSetFillColor(unsigned int rgba)
{
    commands.push_back(new SetFillColor(rgba));
}

void XDot::AddFilledPolygon(int count, wxPoint *points)
{
    commands.push_back(new FilledPolygon(count, points));
}

void XDot::AddBSpline(int count, wxPoint *points)
{
    commands.push_back(new BSpline(count, points));
}

void XDot::AddPolygon(int count, wxPoint *points)
{
    commands.push_back(new Polygon(count, points));
}

void XDot::AddPolyline(int count, wxPoint *points)
{
    commands.push_back(new Polyline(count, points));
}

void XDot::AddSetFont(int family, int style, int weight, int size)
{
    commands.push_back(new SetFont(family, style, weight, size));
}

void XDot::AddText(int x, int y, int alignment, const char *text)
{
    commands.push_back(new Text(x, y, alignment, text));
}

void XDot::AddFilledEllipse(int x, int y, int width, int height)
{
    commands.push_back(new FilledEllipse(x, y, width, height));
}

void XDot::AddEllipse(int x, int y, int width, int height)
{
    commands.push_back(new Ellipse(x, y, width, height));
}

void XDot::Execute(wxDC& dc)
{
    int length = commands.size();
    for (int index = 0; index < length; index++)
        commands[index]->Execute(dc);
}

typedef struct
{
    const char *name;
    unsigned int rgba;
}
X11Color;

static const X11Color
X11ColorTable[] =
{
    { "aliceblue", 0xf0f8ff00 | wxALPHA_OPAQUE },
    { "antiquewhite", 0xfaebd700 | wxALPHA_OPAQUE },
    { "antiquewhite1", 0xffefdb00 | wxALPHA_OPAQUE },
    { "antiquewhite2", 0xeedfcc00 | wxALPHA_OPAQUE },
    { "antiquewhite3", 0xcdc0b000 | wxALPHA_OPAQUE },
    { "antiquewhite4", 0x8b837800 | wxALPHA_OPAQUE },
    { "aquamarine", 0x7fffd400 | wxALPHA_OPAQUE },
    { "aquamarine1", 0x7fffd400 | wxALPHA_OPAQUE },
    { "aquamarine2", 0x76eec600 | wxALPHA_OPAQUE },
    { "aquamarine3", 0x66cdaa00 | wxALPHA_OPAQUE },
    { "aquamarine4", 0x458b7400 | wxALPHA_OPAQUE },
    { "azure", 0xf0ffff00 | wxALPHA_OPAQUE },
    { "azure1", 0xf0ffff00 | wxALPHA_OPAQUE },
    { "azure2", 0xe0eeee00 | wxALPHA_OPAQUE },
    { "azure3", 0xc1cdcd00 | wxALPHA_OPAQUE },
    { "azure4", 0x838b8b00 | wxALPHA_OPAQUE },
    { "beige", 0xf5f5dc00 | wxALPHA_OPAQUE },
    { "bisque", 0xffe4c400 | wxALPHA_OPAQUE },
    { "bisque1", 0xffe4c400 | wxALPHA_OPAQUE },
    { "bisque2", 0xeed5b700 | wxALPHA_OPAQUE },
    { "bisque3", 0xcdb79e00 | wxALPHA_OPAQUE },
    { "bisque4", 0x8b7d6b00 | wxALPHA_OPAQUE },
    { "black", 0x00000000 | wxALPHA_OPAQUE },
    { "blanchedalmond", 0xffebcd00 | wxALPHA_OPAQUE },
    { "blue", 0x0000ff00 | wxALPHA_OPAQUE },
    { "blue1", 0x0000ff00 | wxALPHA_OPAQUE },
    { "blue2", 0x0000ee00 | wxALPHA_OPAQUE },
    { "blue3", 0x0000cd00 | wxALPHA_OPAQUE },
    { "blue4", 0x00008b00 | wxALPHA_OPAQUE },
    { "blueviolet", 0x8a2be200 | wxALPHA_OPAQUE },
    { "brown", 0xa52a2a00 | wxALPHA_OPAQUE },
    { "brown1", 0xff404000 | wxALPHA_OPAQUE },
    { "brown2", 0xee3b3b00 | wxALPHA_OPAQUE },
    { "brown3", 0xcd333300 | wxALPHA_OPAQUE },
    { "brown4", 0x8b232300 | wxALPHA_OPAQUE },
    { "burlywood", 0xdeb88700 | wxALPHA_OPAQUE },
    { "burlywood1", 0xffd39b00 | wxALPHA_OPAQUE },
    { "burlywood2", 0xeec59100 | wxALPHA_OPAQUE },
    { "burlywood3", 0xcdaa7d00 | wxALPHA_OPAQUE },
    { "burlywood4", 0x8b735500 | wxALPHA_OPAQUE },
    { "cadetblue", 0x5f9ea000 | wxALPHA_OPAQUE },
    { "cadetblue1", 0x98f5ff00 | wxALPHA_OPAQUE },
    { "cadetblue2", 0x8ee5ee00 | wxALPHA_OPAQUE },
    { "cadetblue3", 0x7ac5cd00 | wxALPHA_OPAQUE },
    { "cadetblue4", 0x53868b00 | wxALPHA_OPAQUE },
    { "chartreuse", 0x7fff0000 | wxALPHA_OPAQUE },
    { "chartreuse1", 0x7fff0000 | wxALPHA_OPAQUE },
    { "chartreuse2", 0x76ee0000 | wxALPHA_OPAQUE },
    { "chartreuse3", 0x66cd0000 | wxALPHA_OPAQUE },
    { "chartreuse4", 0x458b0000 | wxALPHA_OPAQUE },
    { "chocolate", 0xd2691e00 | wxALPHA_OPAQUE },
    { "chocolate1", 0xff7f2400 | wxALPHA_OPAQUE },
    { "chocolate2", 0xee762100 | wxALPHA_OPAQUE },
    { "chocolate3", 0xcd661d00 | wxALPHA_OPAQUE },
    { "chocolate4", 0x8b451300 | wxALPHA_OPAQUE },
    { "coral", 0xff7f5000 | wxALPHA_OPAQUE },
    { "coral1", 0xff725600 | wxALPHA_OPAQUE },
    { "coral2", 0xee6a5000 | wxALPHA_OPAQUE },
    { "coral3", 0xcd5b4500 | wxALPHA_OPAQUE },
    { "coral4", 0x8b3e2f00 | wxALPHA_OPAQUE },
    { "cornflowerblue", 0x6495ed00 | wxALPHA_OPAQUE },
    { "cornsilk", 0xfff8dc00 | wxALPHA_OPAQUE },
    { "cornsilk1", 0xfff8dc00 | wxALPHA_OPAQUE },
    { "cornsilk2", 0xeee8cd00 | wxALPHA_OPAQUE },
    { "cornsilk3", 0xcdc8b100 | wxALPHA_OPAQUE },
    { "cornsilk4", 0x8b887800 | wxALPHA_OPAQUE },
    { "crimson", 0xdc143c00 | wxALPHA_OPAQUE },
    { "cyan", 0x00ffff00 | wxALPHA_OPAQUE },
    { "cyan1", 0x00ffff00 | wxALPHA_OPAQUE },
    { "cyan2", 0x00eeee00 | wxALPHA_OPAQUE },
    { "cyan3", 0x00cdcd00 | wxALPHA_OPAQUE },
    { "cyan4", 0x008b8b00 | wxALPHA_OPAQUE },
    { "darkgoldenrod", 0xb8860b00 | wxALPHA_OPAQUE },
    { "darkgoldenrod1", 0xffb90f00 | wxALPHA_OPAQUE },
    { "darkgoldenrod2", 0xeead0e00 | wxALPHA_OPAQUE },
    { "darkgoldenrod3", 0xcd950c00 | wxALPHA_OPAQUE },
    { "darkgoldenrod4", 0x8b650800 | wxALPHA_OPAQUE },
    { "darkgreen", 0x00640000 | wxALPHA_OPAQUE },
    { "darkkhaki", 0xbdb76b00 | wxALPHA_OPAQUE },
    { "darkolivegreen", 0x556b2f00 | wxALPHA_OPAQUE },
    { "darkolivegreen1", 0xcaff7000 | wxALPHA_OPAQUE },
    { "darkolivegreen2", 0xbcee6800 | wxALPHA_OPAQUE },
    { "darkolivegreen3", 0xa2cd5a00 | wxALPHA_OPAQUE },
    { "darkolivegreen4", 0x6e8b3d00 | wxALPHA_OPAQUE },
    { "darkorange", 0xff8c0000 | wxALPHA_OPAQUE },
    { "darkorange1", 0xff7f0000 | wxALPHA_OPAQUE },
    { "darkorange2", 0xee760000 | wxALPHA_OPAQUE },
    { "darkorange3", 0xcd660000 | wxALPHA_OPAQUE },
    { "darkorange4", 0x8b450000 | wxALPHA_OPAQUE },
    { "darkorchid", 0x9932cc00 | wxALPHA_OPAQUE },
    { "darkorchid1", 0xbf3eff00 | wxALPHA_OPAQUE },
    { "darkorchid2", 0xb23aee00 | wxALPHA_OPAQUE },
    { "darkorchid3", 0x9a32cd00 | wxALPHA_OPAQUE },
    { "darkorchid4", 0x68228b00 | wxALPHA_OPAQUE },
    { "darksalmon", 0xe9967a00 | wxALPHA_OPAQUE },
    { "darkseagreen", 0x8fbc8f00 | wxALPHA_OPAQUE },
    { "darkseagreen1", 0xc1ffc100 | wxALPHA_OPAQUE },
    { "darkseagreen2", 0xb4eeb400 | wxALPHA_OPAQUE },
    { "darkseagreen3", 0x9bcd9b00 | wxALPHA_OPAQUE },
    { "darkseagreen4", 0x698b6900 | wxALPHA_OPAQUE },
    { "darkslateblue", 0x483d8b00 | wxALPHA_OPAQUE },
    { "darkslategray", 0x2f4f4f00 | wxALPHA_OPAQUE },
    { "darkslategray1", 0x97ffff00 | wxALPHA_OPAQUE },
    { "darkslategray2", 0x8deeee00 | wxALPHA_OPAQUE },
    { "darkslategray3", 0x79cdcd00 | wxALPHA_OPAQUE },
    { "darkslategray4", 0x528b8b00 | wxALPHA_OPAQUE },
    { "darkslategrey", 0x2f4f4f00 | wxALPHA_OPAQUE },
    { "darkturquoise", 0x00ced100 | wxALPHA_OPAQUE },
    { "darkviolet", 0x9400d300 | wxALPHA_OPAQUE },
    { "deeppink", 0xff149300 | wxALPHA_OPAQUE },
    { "deeppink1", 0xff149300 | wxALPHA_OPAQUE },
    { "deeppink2", 0xee128900 | wxALPHA_OPAQUE },
    { "deeppink3", 0xcd107600 | wxALPHA_OPAQUE },
    { "deeppink4", 0x8b0a5000 | wxALPHA_OPAQUE },
    { "deepskyblue", 0x00bfff00 | wxALPHA_OPAQUE },
    { "deepskyblue1", 0x00bfff00 | wxALPHA_OPAQUE },
    { "deepskyblue2", 0x00b2ee00 | wxALPHA_OPAQUE },
    { "deepskyblue3", 0x009acd00 | wxALPHA_OPAQUE },
    { "deepskyblue4", 0x00688b00 | wxALPHA_OPAQUE },
    { "dimgray", 0x69696900 | wxALPHA_OPAQUE },
    { "dimgrey", 0x69696900 | wxALPHA_OPAQUE },
    { "dodgerblue", 0x1e90ff00 | wxALPHA_OPAQUE },
    { "dodgerblue1", 0x1e90ff00 | wxALPHA_OPAQUE },
    { "dodgerblue2", 0x1c86ee00 | wxALPHA_OPAQUE },
    { "dodgerblue3", 0x1874cd00 | wxALPHA_OPAQUE },
    { "dodgerblue4", 0x104e8b00 | wxALPHA_OPAQUE },
    { "firebrick", 0xb2222200 | wxALPHA_OPAQUE },
    { "firebrick1", 0xff303000 | wxALPHA_OPAQUE },
    { "firebrick2", 0xee2c2c00 | wxALPHA_OPAQUE },
    { "firebrick3", 0xcd262600 | wxALPHA_OPAQUE },
    { "firebrick4", 0x8b1a1a00 | wxALPHA_OPAQUE },
    { "floralwhite", 0xfffaf000 | wxALPHA_OPAQUE },
    { "forestgreen", 0x228b2200 | wxALPHA_OPAQUE },
    { "gainsboro", 0xdcdcdc00 | wxALPHA_OPAQUE },
    { "ghostwhite", 0xf8f8ff00 | wxALPHA_OPAQUE },
    { "gold", 0xffd70000 | wxALPHA_OPAQUE },
    { "gold1", 0xffd70000 | wxALPHA_OPAQUE },
    { "gold2", 0xeec90000 | wxALPHA_OPAQUE },
    { "gold3", 0xcdad0000 | wxALPHA_OPAQUE },
    { "gold4", 0x8b750000 | wxALPHA_OPAQUE },
    { "goldenrod", 0xdaa52000 | wxALPHA_OPAQUE },
    { "goldenrod1", 0xffc12500 | wxALPHA_OPAQUE },
    { "goldenrod2", 0xeeb42200 | wxALPHA_OPAQUE },
    { "goldenrod3", 0xcd9b1d00 | wxALPHA_OPAQUE },
    { "goldenrod4", 0x8b691400 | wxALPHA_OPAQUE },
    { "gray", 0xc0c0c000 | wxALPHA_OPAQUE },
    { "gray0", 0x00000000 | wxALPHA_OPAQUE },
    { "gray1", 0x03030300 | wxALPHA_OPAQUE },
    { "gray10", 0x1a1a1a00 | wxALPHA_OPAQUE },
    { "gray100", 0xffffff00 | wxALPHA_OPAQUE },
    { "gray11", 0x1c1c1c00 | wxALPHA_OPAQUE },
    { "gray12", 0x1f1f1f00 | wxALPHA_OPAQUE },
    { "gray13", 0x21212100 | wxALPHA_OPAQUE },
    { "gray14", 0x24242400 | wxALPHA_OPAQUE },
    { "gray15", 0x26262600 | wxALPHA_OPAQUE },
    { "gray16", 0x29292900 | wxALPHA_OPAQUE },
    { "gray17", 0x2b2b2b00 | wxALPHA_OPAQUE },
    { "gray18", 0x2e2e2e00 | wxALPHA_OPAQUE },
    { "gray19", 0x30303000 | wxALPHA_OPAQUE },
    { "gray2", 0x05050500 | wxALPHA_OPAQUE },
    { "gray20", 0x33333300 | wxALPHA_OPAQUE },
    { "gray21", 0x36363600 | wxALPHA_OPAQUE },
    { "gray22", 0x38383800 | wxALPHA_OPAQUE },
    { "gray23", 0x3b3b3b00 | wxALPHA_OPAQUE },
    { "gray24", 0x3d3d3d00 | wxALPHA_OPAQUE },
    { "gray25", 0x40404000 | wxALPHA_OPAQUE },
    { "gray26", 0x42424200 | wxALPHA_OPAQUE },
    { "gray27", 0x45454500 | wxALPHA_OPAQUE },
    { "gray28", 0x47474700 | wxALPHA_OPAQUE },
    { "gray29", 0x4a4a4a00 | wxALPHA_OPAQUE },
    { "gray3", 0x08080800 | wxALPHA_OPAQUE },
    { "gray30", 0x4d4d4d00 | wxALPHA_OPAQUE },
    { "gray31", 0x4f4f4f00 | wxALPHA_OPAQUE },
    { "gray32", 0x52525200 | wxALPHA_OPAQUE },
    { "gray33", 0x54545400 | wxALPHA_OPAQUE },
    { "gray34", 0x57575700 | wxALPHA_OPAQUE },
    { "gray35", 0x59595900 | wxALPHA_OPAQUE },
    { "gray36", 0x5c5c5c00 | wxALPHA_OPAQUE },
    { "gray37", 0x5e5e5e00 | wxALPHA_OPAQUE },
    { "gray38", 0x61616100 | wxALPHA_OPAQUE },
    { "gray39", 0x63636300 | wxALPHA_OPAQUE },
    { "gray4", 0x0a0a0a00 | wxALPHA_OPAQUE },
    { "gray40", 0x66666600 | wxALPHA_OPAQUE },
    { "gray41", 0x69696900 | wxALPHA_OPAQUE },
    { "gray42", 0x6b6b6b00 | wxALPHA_OPAQUE },
    { "gray43", 0x6e6e6e00 | wxALPHA_OPAQUE },
    { "gray44", 0x70707000 | wxALPHA_OPAQUE },
    { "gray45", 0x73737300 | wxALPHA_OPAQUE },
    { "gray46", 0x75757500 | wxALPHA_OPAQUE },
    { "gray47", 0x78787800 | wxALPHA_OPAQUE },
    { "gray48", 0x7a7a7a00 | wxALPHA_OPAQUE },
    { "gray49", 0x7d7d7d00 | wxALPHA_OPAQUE },
    { "gray5", 0x0d0d0d00 | wxALPHA_OPAQUE },
    { "gray50", 0x7f7f7f00 | wxALPHA_OPAQUE },
    { "gray51", 0x82828200 | wxALPHA_OPAQUE },
    { "gray52", 0x85858500 | wxALPHA_OPAQUE },
    { "gray53", 0x87878700 | wxALPHA_OPAQUE },
    { "gray54", 0x8a8a8a00 | wxALPHA_OPAQUE },
    { "gray55", 0x8c8c8c00 | wxALPHA_OPAQUE },
    { "gray56", 0x8f8f8f00 | wxALPHA_OPAQUE },
    { "gray57", 0x91919100 | wxALPHA_OPAQUE },
    { "gray58", 0x94949400 | wxALPHA_OPAQUE },
    { "gray59", 0x96969600 | wxALPHA_OPAQUE },
    { "gray6", 0x0f0f0f00 | wxALPHA_OPAQUE },
    { "gray60", 0x99999900 | wxALPHA_OPAQUE },
    { "gray61", 0x9c9c9c00 | wxALPHA_OPAQUE },
    { "gray62", 0x9e9e9e00 | wxALPHA_OPAQUE },
    { "gray63", 0xa1a1a100 | wxALPHA_OPAQUE },
    { "gray64", 0xa3a3a300 | wxALPHA_OPAQUE },
    { "gray65", 0xa6a6a600 | wxALPHA_OPAQUE },
    { "gray66", 0xa8a8a800 | wxALPHA_OPAQUE },
    { "gray67", 0xababab00 | wxALPHA_OPAQUE },
    { "gray68", 0xadadad00 | wxALPHA_OPAQUE },
    { "gray69", 0xb0b0b000 | wxALPHA_OPAQUE },
    { "gray7", 0x12121200 | wxALPHA_OPAQUE },
    { "gray70", 0xb3b3b300 | wxALPHA_OPAQUE },
    { "gray71", 0xb5b5b500 | wxALPHA_OPAQUE },
    { "gray72", 0xb8b8b800 | wxALPHA_OPAQUE },
    { "gray73", 0xbababa00 | wxALPHA_OPAQUE },
    { "gray74", 0xbdbdbd00 | wxALPHA_OPAQUE },
    { "gray75", 0xbfbfbf00 | wxALPHA_OPAQUE },
    { "gray76", 0xc2c2c200 | wxALPHA_OPAQUE },
    { "gray77", 0xc4c4c400 | wxALPHA_OPAQUE },
    { "gray78", 0xc7c7c700 | wxALPHA_OPAQUE },
    { "gray79", 0xc9c9c900 | wxALPHA_OPAQUE },
    { "gray8", 0x14141400 | wxALPHA_OPAQUE },
    { "gray80", 0xcccccc00 | wxALPHA_OPAQUE },
    { "gray81", 0xcfcfcf00 | wxALPHA_OPAQUE },
    { "gray82", 0xd1d1d100 | wxALPHA_OPAQUE },
    { "gray83", 0xd4d4d400 | wxALPHA_OPAQUE },
    { "gray84", 0xd6d6d600 | wxALPHA_OPAQUE },
    { "gray85", 0xd9d9d900 | wxALPHA_OPAQUE },
    { "gray86", 0xdbdbdb00 | wxALPHA_OPAQUE },
    { "gray87", 0xdedede00 | wxALPHA_OPAQUE },
    { "gray88", 0xe0e0e000 | wxALPHA_OPAQUE },
    { "gray89", 0xe3e3e300 | wxALPHA_OPAQUE },
    { "gray9", 0x17171700 | wxALPHA_OPAQUE },
    { "gray90", 0xe5e5e500 | wxALPHA_OPAQUE },
    { "gray91", 0xe8e8e800 | wxALPHA_OPAQUE },
    { "gray92", 0xebebeb00 | wxALPHA_OPAQUE },
    { "gray93", 0xededed00 | wxALPHA_OPAQUE },
    { "gray94", 0xf0f0f000 | wxALPHA_OPAQUE },
    { "gray95", 0xf2f2f200 | wxALPHA_OPAQUE },
    { "gray96", 0xf5f5f500 | wxALPHA_OPAQUE },
    { "gray97", 0xf7f7f700 | wxALPHA_OPAQUE },
    { "gray98", 0xfafafa00 | wxALPHA_OPAQUE },
    { "gray99", 0xfcfcfc00 | wxALPHA_OPAQUE },
    { "green", 0x00ff0000 | wxALPHA_OPAQUE },
    { "green1", 0x00ff0000 | wxALPHA_OPAQUE },
    { "green2", 0x00ee0000 | wxALPHA_OPAQUE },
    { "green3", 0x00cd0000 | wxALPHA_OPAQUE },
    { "green4", 0x008b0000 | wxALPHA_OPAQUE },
    { "greenyellow", 0xadff2f00 | wxALPHA_OPAQUE },
    { "grey", 0xc0c0c000 | wxALPHA_OPAQUE },
    { "grey0", 0x00000000 | wxALPHA_OPAQUE },
    { "grey1", 0x03030300 | wxALPHA_OPAQUE },
    { "grey10", 0x1a1a1a00 | wxALPHA_OPAQUE },
    { "grey100", 0xffffff00 | wxALPHA_OPAQUE },
    { "grey11", 0x1c1c1c00 | wxALPHA_OPAQUE },
    { "grey12", 0x1f1f1f00 | wxALPHA_OPAQUE },
    { "grey13", 0x21212100 | wxALPHA_OPAQUE },
    { "grey14", 0x24242400 | wxALPHA_OPAQUE },
    { "grey15", 0x26262600 | wxALPHA_OPAQUE },
    { "grey16", 0x29292900 | wxALPHA_OPAQUE },
    { "grey17", 0x2b2b2b00 | wxALPHA_OPAQUE },
    { "grey18", 0x2e2e2e00 | wxALPHA_OPAQUE },
    { "grey19", 0x30303000 | wxALPHA_OPAQUE },
    { "grey2", 0x05050500 | wxALPHA_OPAQUE },
    { "grey20", 0x33333300 | wxALPHA_OPAQUE },
    { "grey21", 0x36363600 | wxALPHA_OPAQUE },
    { "grey22", 0x38383800 | wxALPHA_OPAQUE },
    { "grey23", 0x3b3b3b00 | wxALPHA_OPAQUE },
    { "grey24", 0x3d3d3d00 | wxALPHA_OPAQUE },
    { "grey25", 0x40404000 | wxALPHA_OPAQUE },
    { "grey26", 0x42424200 | wxALPHA_OPAQUE },
    { "grey27", 0x45454500 | wxALPHA_OPAQUE },
    { "grey28", 0x47474700 | wxALPHA_OPAQUE },
    { "grey29", 0x4a4a4a00 | wxALPHA_OPAQUE },
    { "grey3", 0x08080800 | wxALPHA_OPAQUE },
    { "grey30", 0x4d4d4d00 | wxALPHA_OPAQUE },
    { "grey31", 0x4f4f4f00 | wxALPHA_OPAQUE },
    { "grey32", 0x52525200 | wxALPHA_OPAQUE },
    { "grey33", 0x54545400 | wxALPHA_OPAQUE },
    { "grey34", 0x57575700 | wxALPHA_OPAQUE },
    { "grey35", 0x59595900 | wxALPHA_OPAQUE },
    { "grey36", 0x5c5c5c00 | wxALPHA_OPAQUE },
    { "grey37", 0x5e5e5e00 | wxALPHA_OPAQUE },
    { "grey38", 0x61616100 | wxALPHA_OPAQUE },
    { "grey39", 0x63636300 | wxALPHA_OPAQUE },
    { "grey4", 0x0a0a0a00 | wxALPHA_OPAQUE },
    { "grey40", 0x66666600 | wxALPHA_OPAQUE },
    { "grey41", 0x69696900 | wxALPHA_OPAQUE },
    { "grey42", 0x6b6b6b00 | wxALPHA_OPAQUE },
    { "grey43", 0x6e6e6e00 | wxALPHA_OPAQUE },
    { "grey44", 0x70707000 | wxALPHA_OPAQUE },
    { "grey45", 0x73737300 | wxALPHA_OPAQUE },
    { "grey46", 0x75757500 | wxALPHA_OPAQUE },
    { "grey47", 0x78787800 | wxALPHA_OPAQUE },
    { "grey48", 0x7a7a7a00 | wxALPHA_OPAQUE },
    { "grey49", 0x7d7d7d00 | wxALPHA_OPAQUE },
    { "grey5", 0x0d0d0d00 | wxALPHA_OPAQUE },
    { "grey50", 0x7f7f7f00 | wxALPHA_OPAQUE },
    { "grey51", 0x82828200 | wxALPHA_OPAQUE },
    { "grey52", 0x85858500 | wxALPHA_OPAQUE },
    { "grey53", 0x87878700 | wxALPHA_OPAQUE },
    { "grey54", 0x8a8a8a00 | wxALPHA_OPAQUE },
    { "grey55", 0x8c8c8c00 | wxALPHA_OPAQUE },
    { "grey56", 0x8f8f8f00 | wxALPHA_OPAQUE },
    { "grey57", 0x91919100 | wxALPHA_OPAQUE },
    { "grey58", 0x94949400 | wxALPHA_OPAQUE },
    { "grey59", 0x96969600 | wxALPHA_OPAQUE },
    { "grey6", 0x0f0f0f00 | wxALPHA_OPAQUE },
    { "grey60", 0x99999900 | wxALPHA_OPAQUE },
    { "grey61", 0x9c9c9c00 | wxALPHA_OPAQUE },
    { "grey62", 0x9e9e9e00 | wxALPHA_OPAQUE },
    { "grey63", 0xa1a1a100 | wxALPHA_OPAQUE },
    { "grey64", 0xa3a3a300 | wxALPHA_OPAQUE },
    { "grey65", 0xa6a6a600 | wxALPHA_OPAQUE },
    { "grey66", 0xa8a8a800 | wxALPHA_OPAQUE },
    { "grey67", 0xababab00 | wxALPHA_OPAQUE },
    { "grey68", 0xadadad00 | wxALPHA_OPAQUE },
    { "grey69", 0xb0b0b000 | wxALPHA_OPAQUE },
    { "grey7", 0x12121200 | wxALPHA_OPAQUE },
    { "grey70", 0xb3b3b300 | wxALPHA_OPAQUE },
    { "grey71", 0xb5b5b500 | wxALPHA_OPAQUE },
    { "grey72", 0xb8b8b800 | wxALPHA_OPAQUE },
    { "grey73", 0xbababa00 | wxALPHA_OPAQUE },
    { "grey74", 0xbdbdbd00 | wxALPHA_OPAQUE },
    { "grey75", 0xbfbfbf00 | wxALPHA_OPAQUE },
    { "grey76", 0xc2c2c200 | wxALPHA_OPAQUE },
    { "grey77", 0xc4c4c400 | wxALPHA_OPAQUE },
    { "grey78", 0xc7c7c700 | wxALPHA_OPAQUE },
    { "grey79", 0xc9c9c900 | wxALPHA_OPAQUE },
    { "grey8", 0x14141400 | wxALPHA_OPAQUE },
    { "grey80", 0xcccccc00 | wxALPHA_OPAQUE },
    { "grey81", 0xcfcfcf00 | wxALPHA_OPAQUE },
    { "grey82", 0xd1d1d100 | wxALPHA_OPAQUE },
    { "grey83", 0xd4d4d400 | wxALPHA_OPAQUE },
    { "grey84", 0xd6d6d600 | wxALPHA_OPAQUE },
    { "grey85", 0xd9d9d900 | wxALPHA_OPAQUE },
    { "grey86", 0xdbdbdb00 | wxALPHA_OPAQUE },
    { "grey87", 0xdedede00 | wxALPHA_OPAQUE },
    { "grey88", 0xe0e0e000 | wxALPHA_OPAQUE },
    { "grey89", 0xe3e3e300 | wxALPHA_OPAQUE },
    { "grey9", 0x17171700 | wxALPHA_OPAQUE },
    { "grey90", 0xe5e5e500 | wxALPHA_OPAQUE },
    { "grey91", 0xe8e8e800 | wxALPHA_OPAQUE },
    { "grey92", 0xebebeb00 | wxALPHA_OPAQUE },
    { "grey93", 0xededed00 | wxALPHA_OPAQUE },
    { "grey94", 0xf0f0f000 | wxALPHA_OPAQUE },
    { "grey95", 0xf2f2f200 | wxALPHA_OPAQUE },
    { "grey96", 0xf5f5f500 | wxALPHA_OPAQUE },
    { "grey97", 0xf7f7f700 | wxALPHA_OPAQUE },
    { "grey98", 0xfafafa00 | wxALPHA_OPAQUE },
    { "grey99", 0xfcfcfc00 | wxALPHA_OPAQUE },
    { "honeydew", 0xf0fff000 | wxALPHA_OPAQUE },
    { "honeydew1", 0xf0fff000 | wxALPHA_OPAQUE },
    { "honeydew2", 0xe0eee000 | wxALPHA_OPAQUE },
    { "honeydew3", 0xc1cdc100 | wxALPHA_OPAQUE },
    { "honeydew4", 0x838b8300 | wxALPHA_OPAQUE },
    { "hotpink", 0xff69b400 | wxALPHA_OPAQUE },
    { "hotpink1", 0xff6eb400 | wxALPHA_OPAQUE },
    { "hotpink2", 0xee6aa700 | wxALPHA_OPAQUE },
    { "hotpink3", 0xcd609000 | wxALPHA_OPAQUE },
    { "hotpink4", 0x8b3a6200 | wxALPHA_OPAQUE },
    { "indianred", 0xcd5c5c00 | wxALPHA_OPAQUE },
    { "indianred1", 0xff6a6a00 | wxALPHA_OPAQUE },
    { "indianred2", 0xee636300 | wxALPHA_OPAQUE },
    { "indianred3", 0xcd555500 | wxALPHA_OPAQUE },
    { "indianred4", 0x8b3a3a00 | wxALPHA_OPAQUE },
    { "indigo", 0x4b008200 | wxALPHA_OPAQUE },
    { "ivory", 0xfffff000 | wxALPHA_OPAQUE },
    { "ivory1", 0xfffff000 | wxALPHA_OPAQUE },
    { "ivory2", 0xeeeee000 | wxALPHA_OPAQUE },
    { "ivory3", 0xcdcdc100 | wxALPHA_OPAQUE },
    { "ivory4", 0x8b8b8300 | wxALPHA_OPAQUE },
    { "khaki", 0xf0e68c00 | wxALPHA_OPAQUE },
    { "khaki1", 0xfff68f00 | wxALPHA_OPAQUE },
    { "khaki2", 0xeee68500 | wxALPHA_OPAQUE },
    { "khaki3", 0xcdc67300 | wxALPHA_OPAQUE },
    { "khaki4", 0x8b864e00 | wxALPHA_OPAQUE },
    { "lavender", 0xe6e6fa00 | wxALPHA_OPAQUE },
    { "lavenderblush", 0xfff0f500 | wxALPHA_OPAQUE },
    { "lavenderblush1", 0xfff0f500 | wxALPHA_OPAQUE },
    { "lavenderblush2", 0xeee0e500 | wxALPHA_OPAQUE },
    { "lavenderblush3", 0xcdc1c500 | wxALPHA_OPAQUE },
    { "lavenderblush4", 0x8b838600 | wxALPHA_OPAQUE },
    { "lawngreen", 0x7cfc0000 | wxALPHA_OPAQUE },
    { "lemonchiffon", 0xfffacd00 | wxALPHA_OPAQUE },
    { "lemonchiffon1", 0xfffacd00 | wxALPHA_OPAQUE },
    { "lemonchiffon2", 0xeee9bf00 | wxALPHA_OPAQUE },
    { "lemonchiffon3", 0xcdc9a500 | wxALPHA_OPAQUE },
    { "lemonchiffon4", 0x8b897000 | wxALPHA_OPAQUE },
    { "lightblue", 0xadd8e600 | wxALPHA_OPAQUE },
    { "lightblue1", 0xbfefff00 | wxALPHA_OPAQUE },
    { "lightblue2", 0xb2dfee00 | wxALPHA_OPAQUE },
    { "lightblue3", 0x9ac0cd00 | wxALPHA_OPAQUE },
    { "lightblue4", 0x68838b00 | wxALPHA_OPAQUE },
    { "lightcoral", 0xf0808000 | wxALPHA_OPAQUE },
    { "lightcyan", 0xe0ffff00 | wxALPHA_OPAQUE },
    { "lightcyan1", 0xe0ffff00 | wxALPHA_OPAQUE },
    { "lightcyan2", 0xd1eeee00 | wxALPHA_OPAQUE },
    { "lightcyan3", 0xb4cdcd00 | wxALPHA_OPAQUE },
    { "lightcyan4", 0x7a8b8b00 | wxALPHA_OPAQUE },
    { "lightgoldenrod", 0xeedd8200 | wxALPHA_OPAQUE },
    { "lightgoldenrod1", 0xffec8b00 | wxALPHA_OPAQUE },
    { "lightgoldenrod2", 0xeedc8200 | wxALPHA_OPAQUE },
    { "lightgoldenrod3", 0xcdbe7000 | wxALPHA_OPAQUE },
    { "lightgoldenrod4", 0x8b814c00 | wxALPHA_OPAQUE },
    { "lightgoldenrodyellow", 0xfafad200 | wxALPHA_OPAQUE },
    { "lightgray", 0xd3d3d300 | wxALPHA_OPAQUE },
    { "lightgrey", 0xd3d3d300 | wxALPHA_OPAQUE },
    { "lightpink", 0xffb6c100 | wxALPHA_OPAQUE },
    { "lightpink1", 0xffaeb900 | wxALPHA_OPAQUE },
    { "lightpink2", 0xeea2ad00 | wxALPHA_OPAQUE },
    { "lightpink3", 0xcd8c9500 | wxALPHA_OPAQUE },
    { "lightpink4", 0x8b5f6500 | wxALPHA_OPAQUE },
    { "lightsalmon", 0xffa07a00 | wxALPHA_OPAQUE },
    { "lightsalmon1", 0xffa07a00 | wxALPHA_OPAQUE },
    { "lightsalmon2", 0xee957200 | wxALPHA_OPAQUE },
    { "lightsalmon3", 0xcd816200 | wxALPHA_OPAQUE },
    { "lightsalmon4", 0x8b574200 | wxALPHA_OPAQUE },
    { "lightseagreen", 0x20b2aa00 | wxALPHA_OPAQUE },
    { "lightskyblue", 0x87cefa00 | wxALPHA_OPAQUE },
    { "lightskyblue1", 0xb0e2ff00 | wxALPHA_OPAQUE },
    { "lightskyblue2", 0xa4d3ee00 | wxALPHA_OPAQUE },
    { "lightskyblue3", 0x8db6cd00 | wxALPHA_OPAQUE },
    { "lightskyblue4", 0x607b8b00 | wxALPHA_OPAQUE },
    { "lightslateblue", 0x8470ff00 | wxALPHA_OPAQUE },
    { "lightslategray", 0x77889900 | wxALPHA_OPAQUE },
    { "lightslategrey", 0x77889900 | wxALPHA_OPAQUE },
    { "lightsteelblue", 0xb0c4de00 | wxALPHA_OPAQUE },
    { "lightsteelblue1", 0xcae1ff00 | wxALPHA_OPAQUE },
    { "lightsteelblue2", 0xbcd2ee00 | wxALPHA_OPAQUE },
    { "lightsteelblue3", 0xa2b5cd00 | wxALPHA_OPAQUE },
    { "lightsteelblue4", 0x6e7b8b00 | wxALPHA_OPAQUE },
    { "lightyellow", 0xffffe000 | wxALPHA_OPAQUE },
    { "lightyellow1", 0xffffe000 | wxALPHA_OPAQUE },
    { "lightyellow2", 0xeeeed100 | wxALPHA_OPAQUE },
    { "lightyellow3", 0xcdcdb400 | wxALPHA_OPAQUE },
    { "lightyellow4", 0x8b8b7a00 | wxALPHA_OPAQUE },
    { "limegreen", 0x32cd3200 | wxALPHA_OPAQUE },
    { "linen", 0xfaf0e600 | wxALPHA_OPAQUE },
    { "magenta", 0xff00ff00 | wxALPHA_OPAQUE },
    { "magenta1", 0xff00ff00 | wxALPHA_OPAQUE },
    { "magenta2", 0xee00ee00 | wxALPHA_OPAQUE },
    { "magenta3", 0xcd00cd00 | wxALPHA_OPAQUE },
    { "magenta4", 0x8b008b00 | wxALPHA_OPAQUE },
    { "maroon", 0xb0306000 | wxALPHA_OPAQUE },
    { "maroon1", 0xff34b300 | wxALPHA_OPAQUE },
    { "maroon2", 0xee30a700 | wxALPHA_OPAQUE },
    { "maroon3", 0xcd299000 | wxALPHA_OPAQUE },
    { "maroon4", 0x8b1c6200 | wxALPHA_OPAQUE },
    { "mediumaquamarine", 0x66cdaa00 | wxALPHA_OPAQUE },
    { "mediumblue", 0x0000cd00 | wxALPHA_OPAQUE },
    { "mediumorchid", 0xba55d300 | wxALPHA_OPAQUE },
    { "mediumorchid1", 0xe066ff00 | wxALPHA_OPAQUE },
    { "mediumorchid2", 0xd15fee00 | wxALPHA_OPAQUE },
    { "mediumorchid3", 0xb452cd00 | wxALPHA_OPAQUE },
    { "mediumorchid4", 0x7a378b00 | wxALPHA_OPAQUE },
    { "mediumpurple", 0x9370db00 | wxALPHA_OPAQUE },
    { "mediumpurple1", 0xab82ff00 | wxALPHA_OPAQUE },
    { "mediumpurple2", 0x9f79ee00 | wxALPHA_OPAQUE },
    { "mediumpurple3", 0x8968cd00 | wxALPHA_OPAQUE },
    { "mediumpurple4", 0x5d478b00 | wxALPHA_OPAQUE },
    { "mediumseagreen", 0x3cb37100 | wxALPHA_OPAQUE },
    { "mediumslateblue", 0x7b68ee00 | wxALPHA_OPAQUE },
    { "mediumspringgreen", 0x00fa9a00 | wxALPHA_OPAQUE },
    { "mediumturquoise", 0x48d1cc00 | wxALPHA_OPAQUE },
    { "mediumvioletred", 0xc7158500 | wxALPHA_OPAQUE },
    { "midnightblue", 0x19197000 | wxALPHA_OPAQUE },
    { "mintcream", 0xf5fffa00 | wxALPHA_OPAQUE },
    { "mistyrose", 0xffe4e100 | wxALPHA_OPAQUE },
    { "mistyrose1", 0xffe4e100 | wxALPHA_OPAQUE },
    { "mistyrose2", 0xeed5d200 | wxALPHA_OPAQUE },
    { "mistyrose3", 0xcdb7b500 | wxALPHA_OPAQUE },
    { "mistyrose4", 0x8b7d7b00 | wxALPHA_OPAQUE },
    { "moccasin", 0xffe4b500 | wxALPHA_OPAQUE },
    { "navajowhite", 0xffdead00 | wxALPHA_OPAQUE },
    { "navajowhite1", 0xffdead00 | wxALPHA_OPAQUE },
    { "navajowhite2", 0xeecfa100 | wxALPHA_OPAQUE },
    { "navajowhite3", 0xcdb38b00 | wxALPHA_OPAQUE },
    { "navajowhite4", 0x8b795e00 | wxALPHA_OPAQUE },
    { "navy", 0x00008000 | wxALPHA_OPAQUE },
    { "navyblue", 0x00008000 | wxALPHA_OPAQUE },
    { "oldlace", 0xfdf5e600 | wxALPHA_OPAQUE },
    { "olivedrab", 0x6b8e2300 | wxALPHA_OPAQUE },
    { "olivedrab1", 0xc0ff3e00 | wxALPHA_OPAQUE },
    { "olivedrab2", 0xb3ee3a00 | wxALPHA_OPAQUE },
    { "olivedrab3", 0x9acd3200 | wxALPHA_OPAQUE },
    { "olivedrab4", 0x698b2200 | wxALPHA_OPAQUE },
    { "orange", 0xffa50000 | wxALPHA_OPAQUE },
    { "orange1", 0xffa50000 | wxALPHA_OPAQUE },
    { "orange2", 0xee9a0000 | wxALPHA_OPAQUE },
    { "orange3", 0xcd850000 | wxALPHA_OPAQUE },
    { "orange4", 0x8b5a0000 | wxALPHA_OPAQUE },
    { "orangered", 0xff450000 | wxALPHA_OPAQUE },
    { "orangered1", 0xff450000 | wxALPHA_OPAQUE },
    { "orangered2", 0xee400000 | wxALPHA_OPAQUE },
    { "orangered3", 0xcd370000 | wxALPHA_OPAQUE },
    { "orangered4", 0x8b250000 | wxALPHA_OPAQUE },
    { "orchid", 0xda70d600 | wxALPHA_OPAQUE },
    { "orchid1", 0xff83fa00 | wxALPHA_OPAQUE },
    { "orchid2", 0xee7ae900 | wxALPHA_OPAQUE },
    { "orchid3", 0xcd69c900 | wxALPHA_OPAQUE },
    { "orchid4", 0x8b478900 | wxALPHA_OPAQUE },
    { "palegoldenrod", 0xeee8aa00 | wxALPHA_OPAQUE },
    { "palegreen", 0x98fb9800 | wxALPHA_OPAQUE },
    { "palegreen1", 0x9aff9a00 | wxALPHA_OPAQUE },
    { "palegreen2", 0x90ee9000 | wxALPHA_OPAQUE },
    { "palegreen3", 0x7ccd7c00 | wxALPHA_OPAQUE },
    { "palegreen4", 0x548b5400 | wxALPHA_OPAQUE },
    { "paleturquoise", 0xafeeee00 | wxALPHA_OPAQUE },
    { "paleturquoise1", 0xbbffff00 | wxALPHA_OPAQUE },
    { "paleturquoise2", 0xaeeeee00 | wxALPHA_OPAQUE },
    { "paleturquoise3", 0x96cdcd00 | wxALPHA_OPAQUE },
    { "paleturquoise4", 0x668b8b00 | wxALPHA_OPAQUE },
    { "palevioletred", 0xdb709300 | wxALPHA_OPAQUE },
    { "palevioletred1", 0xff82ab00 | wxALPHA_OPAQUE },
    { "palevioletred2", 0xee799f00 | wxALPHA_OPAQUE },
    { "palevioletred3", 0xcd688900 | wxALPHA_OPAQUE },
    { "palevioletred4", 0x8b475d00 | wxALPHA_OPAQUE },
    { "papayawhip", 0xffefd500 | wxALPHA_OPAQUE },
    { "peachpuff", 0xffdab900 | wxALPHA_OPAQUE },
    { "peachpuff1", 0xffdab900 | wxALPHA_OPAQUE },
    { "peachpuff2", 0xeecbad00 | wxALPHA_OPAQUE },
    { "peachpuff3", 0xcdaf9500 | wxALPHA_OPAQUE },
    { "peachpuff4", 0x8b776500 | wxALPHA_OPAQUE },
    { "peru", 0xcd853f00 | wxALPHA_OPAQUE },
    { "pink", 0xffc0cb00 | wxALPHA_OPAQUE },
    { "pink1", 0xffb5c500 | wxALPHA_OPAQUE },
    { "pink2", 0xeea9b800 | wxALPHA_OPAQUE },
    { "pink3", 0xcd919e00 | wxALPHA_OPAQUE },
    { "pink4", 0x8b636c00 | wxALPHA_OPAQUE },
    { "plum", 0xdda0dd00 | wxALPHA_OPAQUE },
    { "plum1", 0xffbbff00 | wxALPHA_OPAQUE },
    { "plum2", 0xeeaeee00 | wxALPHA_OPAQUE },
    { "plum3", 0xcd96cd00 | wxALPHA_OPAQUE },
    { "plum4", 0x8b668b00 | wxALPHA_OPAQUE },
    { "powderblue", 0xb0e0e600 | wxALPHA_OPAQUE },
    { "purple", 0xa020f000 | wxALPHA_OPAQUE },
    { "purple1", 0x9b30ff00 | wxALPHA_OPAQUE },
    { "purple2", 0x912cee00 | wxALPHA_OPAQUE },
    { "purple3", 0x7d26cd00 | wxALPHA_OPAQUE },
    { "purple4", 0x551a8b00 | wxALPHA_OPAQUE },
    { "red", 0xff000000 | wxALPHA_OPAQUE },
    { "red1", 0xff000000 | wxALPHA_OPAQUE },
    { "red2", 0xee000000 | wxALPHA_OPAQUE },
    { "red3", 0xcd000000 | wxALPHA_OPAQUE },
    { "red4", 0x8b000000 | wxALPHA_OPAQUE },
    { "rosybrown", 0xbc8f8f00 | wxALPHA_OPAQUE },
    { "rosybrown1", 0xffc1c100 | wxALPHA_OPAQUE },
    { "rosybrown2", 0xeeb4b400 | wxALPHA_OPAQUE },
    { "rosybrown3", 0xcd9b9b00 | wxALPHA_OPAQUE },
    { "rosybrown4", 0x8b696900 | wxALPHA_OPAQUE },
    { "royalblue", 0x4169e100 | wxALPHA_OPAQUE },
    { "royalblue1", 0x4876ff00 | wxALPHA_OPAQUE },
    { "royalblue2", 0x436eee00 | wxALPHA_OPAQUE },
    { "royalblue3", 0x3a5fcd00 | wxALPHA_OPAQUE },
    { "royalblue4", 0x27408b00 | wxALPHA_OPAQUE },
    { "saddlebrown", 0x8b451300 | wxALPHA_OPAQUE },
    { "salmon", 0xfa807200 | wxALPHA_OPAQUE },
    { "salmon1", 0xff8c6900 | wxALPHA_OPAQUE },
    { "salmon2", 0xee826200 | wxALPHA_OPAQUE },
    { "salmon3", 0xcd705400 | wxALPHA_OPAQUE },
    { "salmon4", 0x8b4c3900 | wxALPHA_OPAQUE },
    { "sandybrown", 0xf4a46000 | wxALPHA_OPAQUE },
    { "seagreen", 0x2e8b5700 | wxALPHA_OPAQUE },
    { "seagreen1", 0x54ff9f00 | wxALPHA_OPAQUE },
    { "seagreen2", 0x4eee9400 | wxALPHA_OPAQUE },
    { "seagreen3", 0x43cd8000 | wxALPHA_OPAQUE },
    { "seagreen4", 0x2e8b5700 | wxALPHA_OPAQUE },
    { "seashell", 0xfff5ee00 | wxALPHA_OPAQUE },
    { "seashell1", 0xfff5ee00 | wxALPHA_OPAQUE },
    { "seashell2", 0xeee5de00 | wxALPHA_OPAQUE },
    { "seashell3", 0xcdc5bf00 | wxALPHA_OPAQUE },
    { "seashell4", 0x8b868200 | wxALPHA_OPAQUE },
    { "sienna", 0xa0522d00 | wxALPHA_OPAQUE },
    { "sienna1", 0xff824700 | wxALPHA_OPAQUE },
    { "sienna2", 0xee794200 | wxALPHA_OPAQUE },
    { "sienna3", 0xcd683900 | wxALPHA_OPAQUE },
    { "sienna4", 0x8b472600 | wxALPHA_OPAQUE },
    { "skyblue", 0x87ceeb00 | wxALPHA_OPAQUE },
    { "skyblue1", 0x87ceff00 | wxALPHA_OPAQUE },
    { "skyblue2", 0x7ec0ee00 | wxALPHA_OPAQUE },
    { "skyblue3", 0x6ca6cd00 | wxALPHA_OPAQUE },
    { "skyblue4", 0x4a708b00 | wxALPHA_OPAQUE },
    { "slateblue", 0x6a5acd00 | wxALPHA_OPAQUE },
    { "slateblue1", 0x836fff00 | wxALPHA_OPAQUE },
    { "slateblue2", 0x7a67ee00 | wxALPHA_OPAQUE },
    { "slateblue3", 0x6959cd00 | wxALPHA_OPAQUE },
    { "slateblue4", 0x473c8b00 | wxALPHA_OPAQUE },
    { "slategray", 0x70809000 | wxALPHA_OPAQUE },
    { "slategray1", 0xc6e2ff00 | wxALPHA_OPAQUE },
    { "slategray2", 0xb9d3ee00 | wxALPHA_OPAQUE },
    { "slategray3", 0x9fb6cd00 | wxALPHA_OPAQUE },
    { "slategray4", 0x6c7b8b00 | wxALPHA_OPAQUE },
    { "slategrey", 0x70809000 | wxALPHA_OPAQUE },
    { "snow", 0xfffafa00 | wxALPHA_OPAQUE },
    { "snow1", 0xfffafa00 | wxALPHA_OPAQUE },
    { "snow2", 0xeee9e900 | wxALPHA_OPAQUE },
    { "snow3", 0xcdc9c900 | wxALPHA_OPAQUE },
    { "snow4", 0x8b898900 | wxALPHA_OPAQUE },
    { "springgreen", 0x00ff7f00 | wxALPHA_OPAQUE },
    { "springgreen1", 0x00ff7f00 | wxALPHA_OPAQUE },
    { "springgreen2", 0x00ee7600 | wxALPHA_OPAQUE },
    { "springgreen3", 0x00cd6600 | wxALPHA_OPAQUE },
    { "springgreen4", 0x008b4500 | wxALPHA_OPAQUE },
    { "steelblue", 0x4682b400 | wxALPHA_OPAQUE },
    { "steelblue1", 0x63b8ff00 | wxALPHA_OPAQUE },
    { "steelblue2", 0x5cacee00 | wxALPHA_OPAQUE },
    { "steelblue3", 0x4f94cd00 | wxALPHA_OPAQUE },
    { "steelblue4", 0x36648b00 | wxALPHA_OPAQUE },
    { "tan", 0xd2b48c00 | wxALPHA_OPAQUE },
    { "tan1", 0xffa54f00 | wxALPHA_OPAQUE },
    { "tan2", 0xee9a4900 | wxALPHA_OPAQUE },
    { "tan3", 0xcd853f00 | wxALPHA_OPAQUE },
    { "tan4", 0x8b5a2b00 | wxALPHA_OPAQUE },
    { "thistle", 0xd8bfd800 | wxALPHA_OPAQUE },
    { "thistle1", 0xffe1ff00 | wxALPHA_OPAQUE },
    { "thistle2", 0xeed2ee00 | wxALPHA_OPAQUE },
    { "thistle3", 0xcdb5cd00 | wxALPHA_OPAQUE },
    { "thistle4", 0x8b7b8b00 | wxALPHA_OPAQUE },
    { "tomato", 0xff634700 | wxALPHA_OPAQUE },
    { "tomato1", 0xff634700 | wxALPHA_OPAQUE },
    { "tomato2", 0xee5c4200 | wxALPHA_OPAQUE },
    { "tomato3", 0xcd4f3900 | wxALPHA_OPAQUE },
    { "tomato4", 0x8b362600 | wxALPHA_OPAQUE },
    { "transparent", 0xfffffe00 | wxALPHA_OPAQUE },
    { "turquoise", 0x40e0d000 | wxALPHA_OPAQUE },
    { "turquoise1", 0x00f5ff00 | wxALPHA_OPAQUE },
    { "turquoise2", 0x00e5ee00 | wxALPHA_OPAQUE },
    { "turquoise3", 0x00c5cd00 | wxALPHA_OPAQUE },
    { "turquoise4", 0x00868b00 | wxALPHA_OPAQUE },
    { "violet", 0xee82ee00 | wxALPHA_OPAQUE },
    { "violetred", 0xd0209000 | wxALPHA_OPAQUE },
    { "violetred1", 0xff3e9600 | wxALPHA_OPAQUE },
    { "violetred2", 0xee3a8c00 | wxALPHA_OPAQUE },
    { "violetred3", 0xcd327800 | wxALPHA_OPAQUE },
    { "violetred4", 0x8b225200 | wxALPHA_OPAQUE },
    { "wheat", 0xf5deb300 | wxALPHA_OPAQUE },
    { "wheat1", 0xffe7ba00 | wxALPHA_OPAQUE },
    { "wheat2", 0xeed8ae00 | wxALPHA_OPAQUE },
    { "wheat3", 0xcdba9600 | wxALPHA_OPAQUE },
    { "wheat4", 0x8b7e6600 | wxALPHA_OPAQUE },
    { "white", 0xffffff00 | wxALPHA_OPAQUE },
    { "whitesmoke", 0xf5f5f500 | wxALPHA_OPAQUE },
    { "yellow", 0xffff0000 | wxALPHA_OPAQUE },
    { "yellow1", 0xffff0000 | wxALPHA_OPAQUE },
    { "yellow2", 0xeeee0000 | wxALPHA_OPAQUE },
    { "yellow3", 0xcdcd0000 | wxALPHA_OPAQUE },
    { "yellow4", 0x8b8b0000 | wxALPHA_OPAQUE },
    { "yellowgreen", 0x9acd3200 | wxALPHA_OPAQUE },
};

static int
CompareX11Colors(const void *e1, const void *e2)
{
    const X11Color *c1 = (const X11Color *)e1;
    const X11Color *c2 = (const X11Color *)e2;
    return strcmp(c1->name, c2->name);
}

static unsigned int
FindX11ColorByName(const char *name)
{
    X11Color key = { name, 0 };
    X11Color *color = (X11Color *)bsearch(&key, X11ColorTable, sizeof(X11ColorTable) / sizeof(X11ColorTable[0]), sizeof(X11ColorTable[0]), CompareX11Colors);
    if (color == NULL)
        return 0;
    return color->rgba;
}

static unsigned int HSV2RGB(double h, double s, double v)
{
    int i;
    double r, g, b, f, p, q, t;

    if (s == 0)
        // achromatic (grey)
        r = g = b = v;
    else
    {
        h *= 6; // sector 0 to 5
        i = (int)floor(h);
        f = h - i; // factorial part of h
        p = v * (1 - s);
        q = v * (1 - s * f);
        t = v * (1 - s * (1 - f));
        switch (i) {
            case 0:
                r = v; g = t; b = p; break;
            case 1:
                r = q; g = v; b = p; break;
            case 2:
                r = p; g = v; b = t; break;
            case 3:
                r = p; g = q; b = v; break;
            case 4:
                r = t; g = p; b = v; break;
            default: // case 5:
                r = v; g = p; b = q; break;
        }
    }
    int ir = (int)floor(r * 255);
    int ig = (int)floor(g * 255);
    int ib = (int)floor(b * 255);
    return ir << 24 | ig << 16 | ib << 8 | wxALPHA_OPAQUE;
}

typedef struct
{
    const char *name;
    int family, style, weight;
}
GraphVizFont;

static const
GraphVizFont GraphVizFontTable[] =
{
    { "Courier",                    wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL },
    { "Courier-Bold",                wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD },
    { "Courier-BoldOblique",            wxFONTFAMILY_MODERN, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD },
    { "Courier-Oblique",            wxFONTFAMILY_MODERN, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL },
    { "Courier-Outline",            wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL },
    { "Helvetica",                wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL },
    { "Helvetica-Bold",            wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD },
    { "Helvetica-BoldOblique",        wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD },
    { "Helvetica-Oblique",            wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL },
    { "Helvetica-Outline",            wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL },
    { "Times-Roman",                wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL },
    { "Times-Roman-Bold",            wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD },
    { "Times-Roman-BoldOblique",    wxFONTFAMILY_ROMAN, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD },
    { "Times-Roman-Oblique",        wxFONTFAMILY_ROMAN, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL },
    { "Times-Roman-Outline",        wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL },
};

static int
CompareGraphVizFonts(const void *e1, const void *e2)
{
    const GraphVizFont *f1 = (const GraphVizFont *)e1;
    const GraphVizFont *f2 = (const GraphVizFont *)e2;
    return strcmp(f1->name, f2->name);
}

static const GraphVizFont *
FindGraphVizFontByName(const char *name)
{
    GraphVizFont key = { name, 0 };
    GraphVizFont *font = (GraphVizFont *)bsearch(&key, GraphVizFontTable, sizeof(GraphVizFontTable) / sizeof(GraphVizFontTable[0]), sizeof(GraphVizFontTable[0]), CompareGraphVizFonts);
    if (font != NULL)
        return font;
    return &GraphVizFontTable[5];
}

static bool
Skip(const char **str)
{
    while (**str != '\0' && isspace(**str))
        (*str)++;
    return **str == '\0';
}

static bool
GetInt(const char **str, int *x)
{
    char *last;
    *x = strtol(*str, &last, 10);
    bool error = *str == last;
    *str = last;
    return error;
}

static bool
GetDouble(const char **str, double *x)
{
    char *last;
    *x = strtod(*str, &last);
    bool error = *str == last;
    *str = last;
    return error;
}

static bool
GetStr(const char **str, char *text, int len)
{
    //if (strlen(*str) < len)
    //    return true;
    strncpy(text, *str, len);
    text[len] = '\0';
    (*str) += len;
    return false;
}

void XDot::ParseDrawingCmds(const char *cmds, const char *end)
{
    unsigned int pen_color = 0;
    int pen_width = 1;
    int pen_style = wxSOLID;
    int pen_visible = 1;
    AddSetPen(pen_color, pen_width, pen_style, pen_visible);
    AddSetFillColor(0xffffff00 | wxALPHA_TRANSPARENT);
    AddSetFont(wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, 12);
    while (cmds < end)
    {
        if (Skip(&cmds))
            return;
        char cmd = *cmds++;
        if (Skip(&cmds))
            return;
        switch (cmd)
        {
            case 'c':
            case 'C':
            // c n -c1c2...cn
            // Set pen color. The color value consists of the n characters following '-'.
            // C n -c1c2...cn
            // Set fill color. The color value consists of the n characters following '-'.
            {
                int len;
                if (GetInt(&cmds, &len) || Skip(&cmds))
                    return;
                char name[len + 1];
                if (GetStr(&++cmds, name, len))
                    return;
                unsigned int rgba;
                if (name[0] == '#')
                {
                    if (len == 6)
                        rgba = ((unsigned int)strtol(name, NULL, 16)) << 8 | wxALPHA_OPAQUE;
                    else
                        rgba = (unsigned int)strtol(name, NULL, 16);
                }
                else if (isdigit(name[0]))
                {
                    const char *aux = name;
                    double h, s, v;
                    if (GetDouble(&aux, &h) || Skip(&++aux) || GetDouble(&aux, &s) || Skip(&++aux) || GetDouble(&aux, &v))
                        return;
                    rgba = HSV2RGB(h, s, v);
                }
                else
                    rgba = FindX11ColorByName(name);
                if (cmd == 'c')
                {
                    pen_color = rgba;
                    AddSetPen(pen_color, pen_width, pen_style, pen_visible);
                }
                else
                    AddSetFillColor(rgba);
                break;
            }
            case 'P':
            case 'B':
            case 'p':
            case 'L':
            // P n x1 y1 ... xn yn
            // Filled polygon using the given n points
            // B n x1 y1 ... xn yn
            // B-spline using the given n control points
            // p n x1 y1 ... xn yn
            // Unfilled polygon using the given n points
            // L n x1 y1 ... xn yn
            // Polyline using the given n points
            {
                int count;
                if (GetInt(&cmds, &count))
                    return;
                wxPoint *points = new wxPoint[count];
                for (int i = 0; i < count; i++)
                {
                    int x, y;
                    if (Skip(&cmds) || GetInt(&cmds, &x) || Skip(&cmds) || GetInt(&cmds, &y))
                        return;
                    points[i].x = x;
                    points[i].y = y;
                }
                if (cmd == 'P')
                    AddFilledPolygon(count, points);
                else if (cmd == 'B')
                    AddBSpline(count, points);
                else if (cmd == 'p')
                    AddPolygon(count, points);
                else
                    AddPolyline(count, points);
                break;
            }
            case 'F':
            // F s n -c1c2...cn
            // Set font. The font size is s points. The font name consists of the n characters following '-'.
            {
                double size;
                int len;
                if (GetDouble(&cmds, &size) || Skip(&cmds) || GetInt(&cmds, &len) || Skip(&cmds))
                    return;
                char name[len + 1];
                if (GetStr(&++cmds, name, len))
                    return;
                // Fonts are a bit big for the nodes, subtracting two seems to solve the problem :-p
                size -= 2;
                const GraphVizFont *gvfont = FindGraphVizFontByName(name);
                if (gvfont != NULL)
                    AddSetFont(gvfont->family, gvfont->style, gvfont->weight, (int)size);
                break;
            }
            case 'T':
            // T x y j w n -c1c2...cn
            // Text drawn using the baseline point (x,y). The text consists of the n characters following '-'.
            // The text should be left-aligned (centered, right-aligned) on the point if j is -1 (0, 1), respectively.
            // The value w gives the width of the text as computed by the library.
            {
                int x, y, alignment, len;
                if (
                    GetInt(&cmds, &x) ||
                    Skip(&cmds) ||
                    GetInt(&cmds, &y) ||
                    Skip(&cmds) ||
                    GetInt(&cmds, &alignment) ||
                    Skip(&cmds) ||
                    GetInt(&cmds, &len) || // Skip text width hint
                    Skip(&cmds) ||
                    GetInt(&cmds, &len) ||
                    Skip(&cmds)
                )
                    return;
                char *text = new char[len + 1];
                if (GetStr(&++cmds, text, len))
                    return;
                AddText(x, y, alignment, text);
                break;
            }
            case 'S':
            // S n -c1c2...cn
            // Set style attribute. The style value consists of the n characters following '-'.
            // The syntax of the value is the same as specified for a styleItem in style.
            // http://www.graphviz.org/doc/info/attrs.html#k:style
            // "dashed", "dotted", "solid", "invis", "bold" ( = setlinewidth(2)) for nodes and edges, and
            // "filled", "diagonals" and "rounded" for nodes only.
            // The styles "filled" and "rounded" are recognized for clusters.
            {
                int len;
                if (GetInt(&cmds, &len) || Skip(&cmds))
                    return;
                char style[len + 1];
                if (GetStr(&++cmds, style, len))
                    return;
                if (!strcmp(style, "solid"))
                {
                    pen_visible = pen_style = wxSOLID;
                    AddSetPen(pen_color, pen_width, pen_style, pen_visible);
                }
                else if (!strcmp(style, "dashed"))
                {
                    pen_visible = pen_style = wxSHORT_DASH;
                    AddSetPen(pen_color, pen_width, pen_style, pen_visible);
                }
                else if (!strcmp(style, "dotted"))
                {
                    pen_visible = pen_style = wxDOT;
                    AddSetPen(pen_color, pen_width, pen_style, pen_visible);
                }
                else if (!strcmp(style, "invis"))
                {
                    pen_visible = 0;
                    AddSetPen(pen_color, pen_width, pen_style, pen_visible);
                }
                else if (!strcmp(style, "bold"))
                {
                    pen_width = 2;
                    AddSetPen(pen_color, pen_width, pen_style, pen_visible);
                }
                else if (!strncmp(style, "setlinewidth(", sizeof("setlinewidth(") - 1))
                {
                    int pen_width = (int)atol(style + sizeof("setlinewidth(") - 1);
                    AddSetPen(pen_color, pen_width, pen_style, pen_visible);
                }
                break;
            }
            case 'e':
            case 'E':
            // E x0 y0 w h
            // Filled ellipse ((x-x0)/w)2 + ((y-y0)/h)2 = 1
            // e x0 y0 w h
            // Unfilled ellipse ((x-x0)/w)2 + ((y-y0)/h)2 = 1
            {
                int x0, y0, w, h;
                if (GetInt(&cmds, &x0) || Skip(&cmds) || GetInt(&cmds, &y0) || Skip(&cmds) || GetInt(&cmds, &w) || Skip(&cmds) || GetInt(&cmds, &h))
                    return;
                x0 -= w;
                y0 -= h;
                w += w;
                h += h;
                if (cmd == 'E')
                    AddFilledEllipse(x0, y0, w, h);
                else
                    AddEllipse(x0, y0, w, h);
            }
        }
    }
}

void XDot::Parse(const char *xdot)
{
    // Clear any previous commands.
    Clear();
    // First we parse everything that is not text.
    const char *draw = xdot;
    for (;;)
    {
        const char *cmds = strstr(draw, "draw_");
        if (cmds == NULL)
            break;
        int left = cmds - draw;
        if (
            (left >= 1 && cmds[-1] == '_') ||
            (left >= 2 && cmds[-2] == '_' && (cmds[-1] == 'h' || cmds[-1] == 't'))
        ) {
            const char *begin = strchr(cmds, '"') + 1;
            const char *end = strchr(begin, '"');
            draw = end + 1;
            ParseDrawingCmds(begin, end);
        } else
            draw = cmds + 1;
    }
    // Now we parse just the text commands.
    draw = xdot;
    for (;;)
    {
        const char *cmds = strstr(draw, "draw_");
        if (cmds == NULL)
            break;
        int left = cmds - draw;
        if (
            (left >= 2 && cmds[-2] == '_' && cmds[-1] == 'l') ||
            (left >= 3 && cmds[-3] == '_' && cmds[-1] == 'l' && (cmds[-2] == 'h' || cmds[-2] == 't'))
        ) {
            const char *begin = strchr(cmds, '"') + 1;
            const char *end = strchr(begin, '"');
            draw = end + 1;
            ParseDrawingCmds(begin, end);
        } else
            draw = cmds + 1;
    }
}
