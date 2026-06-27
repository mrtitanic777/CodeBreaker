#ifndef MOUSE_H
#define MOUSE_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for org.lwjgl.input.Mouse.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>

namespace Mouse {
	inline void create(...) {}
	inline void destroy(...) {}
	inline void setNativeCursor(...) {}
	inline void setGrabbed(...) {}
	inline void setCursorPosition(...) {}
	inline int getDX(...) { return 0; }
	inline int getDY(...) { return 0; }
	inline int getX(...) { return 0; }
	inline int getY(...) { return 0; }
	inline int getEventButton(...) { return 0; }
	inline bool getEventButtonState(...) { return false; }
	inline int getEventDWheel(...) { return 0; }
	inline int getEventX(...) { return 0; }
	inline int getEventY(...) { return 0; }
	inline bool isButtonDown(...) { return false; }
	inline bool next(...) { return false; }
}
#endif // MOUSE_H
