#ifndef CONTROLLERS_H
#define CONTROLLERS_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for org.lwjgl.input.Controllers.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>

namespace Controllers {
	inline void create(...) {}
	inline void destroy(...) {}
	inline void poll(...) {}
	inline void clearEvents(...) {}
	inline bool isCreated(...) { return false; }
	inline int getControllerCount(...) { return 0; }
	inline void* getController(...) { return 0; }
	inline bool next(...) { return false; }
	inline void* getEventSource(...) { return 0; }
	inline int getEventControlIndex(...) { return 0; }
	inline bool getEventButtonState(...) { return false; }
}
#endif // CONTROLLERS_H
