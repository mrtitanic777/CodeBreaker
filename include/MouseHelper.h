#ifndef MOUSEHELPER_H
#define MOUSEHELPER_H

// Auto-generated from MouseHelper.java
// Target: Xbox 360 / C++03 / DirectX 9

class Cursor;
class Robot;

class MouseHelper {
public:
	MouseHelper();
	MouseHelper(void* /* PORT: was AWT Component */ var1);
	virtual ~MouseHelper();

	int m_deltaX;
	int m_deltaY;

	virtual void grabMouseCursor();
	virtual void ungrabMouseCursor();

private:
	void* /* PORT: was AWT Component */ m_windowComponent;
	Robot* m_robot;
	int m_mouseX;
	int m_mouseY;
	void* /* PORT: was LWJGL Cursor */ m_cursor;
	int m_mouseInt;

};

#endif // MOUSEHELPER_H