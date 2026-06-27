#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from MouseHelper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MouseHelper.h"
#include "ByteBuffer.h"
#include "Component.h"
#include "Cursor.h"
#include "FloatBuffer.h"
#include "GLAllocation.h"
#include "IntBuffer.h"
#include "LWJGLException.h"
#include "Mouse.h"

// Mouse::setGrabbed / setCursorPosition (LWJGL native cursor grab) are now in the central Mouse
// runtime stub (no-op there; the real impl is the Xbox Input layer, docs/platform/Input.reference).
// The local shim that used to live here was removed -- it would collide with the real Mouse impl.

MouseHelper::MouseHelper(Component* var1)
	: component(NULL)
	, cursor(NULL)
	, deltaX(0)
	, deltaY(0)
	, field_1115_e(0)
{
	this->field_1115_e = 10;

	this->component = var1;
	IntBuffer* var2 = GLAllocation::createDirectIntBuffer(1);
	var2->put(0);
	var2->flip();
	IntBuffer* var3 = GLAllocation::createDirectIntBuffer(1024);
	try {
	this->cursor = new Cursor(32, 32, 16, 16, 1, var3, var2);
	} catch (LWJGLException var5) {
	var5.printStackTrace();
	}
}

MouseHelper::~MouseHelper()
{
	delete cursor;
	cursor = NULL;
}

void MouseHelper::grabMouseCursor()
{
	Mouse::setGrabbed(true);
	this->deltaX = 0;
	this->deltaY = 0;
}

void MouseHelper::ungrabMouseCursor()
{
	Mouse::setCursorPosition(this->component->getWidth() / 2, this->component->getHeight() / 2);
	Mouse::setGrabbed(false);
}

void MouseHelper::updateMouseDelta()
{
	this->deltaX = Mouse::getDX();
	this->deltaY = Mouse::getDY();
}
