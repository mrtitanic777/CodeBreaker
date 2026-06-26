#include "stdafx.h"
// Auto-generated from MouseHelper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MouseHelper.h"
#include "Common.h"
#include "Robot.h"

MouseHelper::MouseHelper()
	: m_windowComponent(NULL)
	, m_robot(NULL)
	, m_mouseX(0)
	, m_mouseY(0)
	, m_cursor(NULL)
	, m_deltaX(0)
	, m_deltaY(0)
	, m_mouseInt(10)
{
}

// PORT: Java Swing/AWT: MouseHelper::MouseHelper(void* /* PORT: was AWT Component */ var1)
	: m_windowComponent(NULL)
	, m_robot(NULL)
	, m_mouseX(0)
	, m_mouseY(0)
	, m_cursor(NULL)
	, m_deltaX(0)
	, m_deltaY(0)
	, m_mouseInt(10)
{
		m_windowComponent = var1;

  { // was try
			m_robot = new Robot();
		} { DbgPrintF("CATCH AWTException\n"); // was catch(AWTException var4)
   // Removed Java stream/exception call: var4.printStackTrace();
		}

		IntBuffer var5 = new int[1];
  var5[0] = 0;
  // NIO flip() — no-op for C array
		IntBuffer var2 = new int[1024];

  { // was try
			m_cursor = new Cursor(32, 32, 16, 16, 1, var2, var5);
		} { DbgPrintF("CATCH LWJGLException\n"); // was catch(LWJGLException var3)
   // Removed Java stream/exception call: var3.printStackTrace();
		}
}

MouseHelper::~MouseHelper() {
	m_windowComponent = NULL;
	m_robot = NULL;
	m_cursor = NULL;
}

void MouseHelper::grabMouseCursor() {
  { // was try
			Mouse.setNativeCursor(m_cursor);
		} { DbgPrintF("CATCH LWJGLException\n"); // was catch(LWJGLException var2)
			var2->printStackTrace();
		}

		ungrabMouseCursor();
		m_deltaX = 0;
		m_deltaY = 0;
}

void MouseHelper::ungrabMouseCursor() {
		Point var1 = MouseInfo::getPointerInfo().getLocation();
		Point var2 = m_windowComponent->getLocationOnScreen();
		m_robot->mouseMove(m_mouseX, m_mouseY);
		m_mouseX = var2->m_x + m_windowComponent->getWidth() / 2;
		m_mouseY = var2->m_y + m_windowComponent->getHeight() / 2;
		if(m_mouseInt == 0) {
			m_deltaX = var1->m_x - m_mouseX;
			m_deltaY = var1->m_y - m_mouseY;
		} else {
			m_deltaX = m_deltaY = 0;
			--m_mouseInt;
		}
}
