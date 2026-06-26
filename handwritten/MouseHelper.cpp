#include "stdafx.h"
// Hand-written PORT override for MouseHelper.java
// -----------------------------------------------
// The Java MouseHelper wraps AWT/LWJGL mouse capture (Robot, Cursor, MouseInfo,
// native cursor grab) to produce a frame look-delta. On Xbox 360 there is no
// mouse; look control comes from the right thumbstick, which the input layer
// writes directly into m_deltaX / m_deltaY each frame. MouseHelper therefore
// becomes a trivial delta holder: grab/ungrab simply reset the accumulated delta.
// The converter-generated MouseHelper.h interface is preserved unchanged.

#include "MouseHelper.h"
#include "Common.h"

MouseHelper::MouseHelper()
	: m_deltaX(0)
	, m_deltaY(0)
	, m_windowComponent(NULL)
	, m_robot(NULL)
	, m_mouseX(0)
	, m_mouseY(0)
	, m_cursor(NULL)
	, m_mouseInt(10)
{
}

MouseHelper::MouseHelper(void* var1)
	: m_deltaX(0)
	, m_deltaY(0)
	, m_windowComponent(var1)
	, m_robot(NULL)
	, m_mouseX(0)
	, m_mouseY(0)
	, m_cursor(NULL)
	, m_mouseInt(10)
{
}

MouseHelper::~MouseHelper() {
	m_windowComponent = NULL;
	m_robot = NULL;
	m_cursor = NULL;
}

void MouseHelper::grabMouseCursor() {
	// Capture begins — clear any stale delta. The thumbstick look-delta is
	// supplied by the input layer (see Minecraft input handling / XInput).
	m_deltaX = 0;
	m_deltaY = 0;
}

void MouseHelper::ungrabMouseCursor() {
	m_deltaX = 0;
	m_deltaY = 0;
}
