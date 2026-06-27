#ifndef MOUSEHELPER_H
#define MOUSEHELPER_H

// Auto-generated from MouseHelper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class Component;
class Cursor;

class MouseHelper {
public:
	MouseHelper(Component* var1);
	virtual ~MouseHelper();

	Component* component;
	Cursor* cursor;
	int deltaX;
	int deltaY;
	int field_1115_e;

	virtual void grabMouseCursor();
	virtual void ungrabMouseCursor();
	virtual void updateMouseDelta();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	MouseHelper(const MouseHelper&);
	MouseHelper& operator=(const MouseHelper&);
};

#endif // MOUSEHELPER_H