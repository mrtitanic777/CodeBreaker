#ifndef DISPLAYMODE_H
#define DISPLAYMODE_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for org.lwjgl.opengl.DisplayMode.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>

class DisplayMode {
public:
	DisplayMode(...) {}
	virtual ~DisplayMode() {}
	virtual int getWidth(...) { return 0; }
	virtual int getHeight(...) { return 0; }
	virtual int getFrequency(...) { return 0; }
	virtual int getBitsPerPixel(...) { return 0; }
};
#endif // DISPLAYMODE_H
