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

// --- Local shim (runtime-stub gap; see final report) ---
// The Mouse runtime stub lacks setGrabbed / setCursorPosition (LWJGL Mouse).
// Reopen the namespace to add no-op shims so the call sites stay unchanged.
// The converter owner should add these centrally with the real implementation.
/* ---- ORIGINAL JAVA (stubbed below) ----
 *  // org.lwjgl.input.Mouse.setGrabbed / setCursorPosition are real LWJGL native calls;
 *  // here they are replaced by the no-op shims declared just below.
 *  public void grabMouseCursor() {
 *     Mouse.setGrabbed(true);
 *     this.deltaX = 0;
 *     this.deltaY = 0;
 *  }
 *
 *  public void ungrabMouseCursor() {
 *     Mouse.setCursorPosition(this.component.getWidth() / 2, this.component.getHeight() / 2);
 *     Mouse.setGrabbed(false);
 *  }
 *  ---- end original Java ---- */
namespace Mouse {
	inline void setGrabbed(bool) {}
	inline void setCursorPosition(int, int) {}
}

MouseHelper::MouseHelper(Component* var1)
	: component(NULL)
	, cursor(NULL)
	, deltaX(0)
	, deltaY(0)
	, field_1115_e(0)
{
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
