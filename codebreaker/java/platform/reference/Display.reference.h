// Display.h -- game-AGNOSTIC org.lwjgl.opengl.Display -> Direct3D 9 for the CodeBreaker Xbox 360 target.
//
// This is the DEVICE OWNER. In the agnostic pipeline the generic entry point just runs static init and
// calls the game's main(); the game then drives display setup itself via LWJGL (Display.setDisplayMode
// -> Display.create -> the render loop -> Display.update). So Display::create() is what actually creates
// the D3D9 device, hands it to the GL emulator (GL11::glSetDevice) and initializes GL, and
// Display::update() presents each frame. No game-specific bootstrap, no Beta Common.h global device.

#ifndef DISPLAY_H
#define DISPLAY_H

#include "DisplayMode.h"

namespace Display {
	// setup
	void setDisplayMode(DisplayMode* mode);
	void setDisplayMode(const DisplayMode& mode);
	void create();
	void destroy();
	// frame
	void update();          // LWJGL: pump input + present the backbuffer
	void swapBuffers();     // present without polling
	// queries
	bool isCloseRequested();
	bool isActive();
	bool isCreated();
	int  getWidth();
	int  getHeight();
	DisplayMode getDisplayMode();
	DisplayMode getDesktopDisplayMode();
	// no-ops on console (window-manager surface area the game may still call)
	inline void setTitle(...) {}
	inline void setFullscreen(...) {}
	inline void setParent(...) {}
	inline void setVSyncEnabled(...) {}
	inline void setIcon(...) {}
	inline void sync(...) {}
	inline void setResizable(...) {}
	inline void setLocation(...) {}
	inline void releaseContext(...) {}
	inline void makeCurrent(...) {}
	inline bool isDirty(...) { return false; }
	inline void* getAvailableDisplayModes(...) { return 0; }
}

#endif // DISPLAY_H
