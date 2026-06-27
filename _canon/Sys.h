#ifndef SYS_H
#define SYS_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for org.lwjgl.Sys.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>

namespace Sys {
	inline void openURL(...) {}
	inline void alert(...) {}
	inline std::string getVersion(...) { return std::string(); }
	inline long long getTime(...) { return 0LL; }
	inline long long getTimerResolution(...) { return 1000LL; }
}
#endif // SYS_H
