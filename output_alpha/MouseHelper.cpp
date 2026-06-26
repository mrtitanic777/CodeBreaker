#include "stdafx.h"
// Hand-written PORT override for MouseHelper.java (Minecraft Alpha v1.2.6)
// -----------------------------------------------------------------------
// The Java MouseHelper wraps AWT/LWJGL mouse capture (Component, LWJGL Cursor,
// Mouse.setGrabbed / Mouse.getDX / Mouse.getDY) to produce a per-frame look-delta.
// On Xbox 360 there is no mouse; look control comes from the right thumbstick,
// which the input layer (XInput) writes directly into m_field_1114_a / m_field_1113_b
// each frame. MouseHelper therefore becomes a trivial delta holder:
//   func_774_a (grab)   -> reset the accumulated delta
//   func_773_b (ungrab) -> reset the accumulated delta
//   func_772_c (update) -> no-op; the XInput layer already supplied the delta
// Modeled on the indev MouseHelper override, adapted to Alpha's obfuscated
// MouseHelper.h interface (field names / func_XXX) which is preserved unchanged.

#include "MouseHelper.h"
#include "Common.h"

MouseHelper::MouseHelper()
	: m_field_1114_a(0)
	, m_field_1113_b(0)
	, m_field_1117_c(NULL)
	, m_field_1116_d(NULL)
	, m_field_1115_e(10)
{
}

MouseHelper::MouseHelper(void* var1)
	: m_field_1114_a(0)
	, m_field_1113_b(0)
	, m_field_1117_c(var1)   // was AWT Component; the LWJGL Cursor creation is dropped (no native cursor on 360)
	, m_field_1116_d(NULL)
	, m_field_1115_e(10)
{
}

MouseHelper::~MouseHelper() {
	m_field_1117_c = NULL;
	m_field_1116_d = NULL;
}

// Java: Mouse.setGrabbed(true); field_1114_a = 0; field_1113_b = 0;
void MouseHelper::func_774_a() {
	// Capture begins -- clear any stale delta. The thumbstick look-delta is
	// supplied by the input layer (see Minecraft input handling / XInput).
	m_field_1114_a = 0;
	m_field_1113_b = 0;
}

// Java: Mouse.setCursorPosition(center); Mouse.setGrabbed(false);
void MouseHelper::func_773_b() {
	// No native cursor to recenter / release on 360. Just drop the delta.
	m_field_1114_a = 0;
	m_field_1113_b = 0;
}

// Java: field_1114_a = Mouse.getDX(); field_1113_b = Mouse.getDY();
void MouseHelper::func_772_c() {
	// On 360 the XInput layer writes the per-frame thumbstick look-delta directly
	// into m_field_1114_a / m_field_1113_b before this is read, so reading the
	// (non-existent) mouse here is a no-op -- leave the supplied delta in place.
}
