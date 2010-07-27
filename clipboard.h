#ifndef __CLIPBOARD_H__
#define __CLIPBOARD_H__

#include "shape.h"
#include "util.h"

#include "debug.h"

class Clipboard: public Shape
{
public:
	// "Regular" constructor.
	Clipboard(): Shape() {}
	// Virtual destructor.
	virtual ~Clipboard()
	{
		List<Shape *> children;
		for (List<Shape *>::Iterator i = ChildIterator(); !i; i++)
		{
			children.Add(*i);
		}
		for (List<Shape *>::Iterator i = children.Iterate(); !i; i++)
		{
			DeleteChild(*i);
		}
	}

	// Adds a shape as a child of this shape without actually changing the parentship.
	virtual void AddChild(Shape *child)
	{
		Shape *parent = child->GetParent();
		Shape::AddChild(child);
		child->SetParent(parent);
	}

	// The class name used in serialization.
	virtual wxString   GetClassName() const          { return wxT("clipboard"); }
	// Creates an instance of this shape.
	static Persistent *Create(const wxXmlNode& root) { return NEW(Clipboard, ()); }
};

#endif // __CLIPBOARD_H__
