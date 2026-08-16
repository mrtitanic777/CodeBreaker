#ifndef DISPLAY_H
#define DISPLAY_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for org.lwjgl.opengl.Display.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>
#include "DisplayMode.h"
#include "Common.h"          // g_pd3dDevice (D3D9 device) -- real frame presentation

// J2X-BUGFIX input-not-pumped: the game never called Input::poll(), so the virtual Mouse/Keyboard event
// queues were never filled and NO input (controller or otherwise) reached the menus. LWJGL's
// Display.update() is the once-per-frame pump point -> poll the pad here.
namespace Input { void poll(); }

namespace Display {
	inline void create(...) {}
	inline void destroy(...) {}
	inline void setDisplayMode(...) {}
	inline void setFullscreen(...) {}
	inline void setParent(...) {}
	inline void setTitle(...) {}
	// End the current scene, FLIP the backbuffer to screen, begin the next scene. Without this the
	// GL->D3D9 emulator draws into the backbuffer but nothing is ever Present()ed -> black screen.
	// (Main.cpp opens the first scene with BeginScene() before the game loop.)
	inline void swapBuffers(...) {
		if (g_pd3dDevice) {
			g_pd3dDevice->EndScene();
			g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
			g_pd3dDevice->BeginScene();
		}
	}
	inline void update(...) { Input::poll(); swapBuffers(); }   // LWJGL update = pump input (once/frame) + present
	inline void setVSyncEnabled(...) {}
	inline void setIcon(...) {}
	inline void sync(...) {}
	inline void setResizable(...) {}
	inline void setLocation(...) {}
	inline void releaseContext(...) {}
	inline void makeCurrent(...) {}
	inline bool isActive(...) { return true; }   // the console IS the foreground app -> always active (focus/mouse-grab gate)
	inline bool isCloseRequested(...) { return false; }
	inline bool isCreated(...) { return false; }
	inline bool isDirty(...) { return false; }
	inline int getWidth(...) { return 0; }
	inline int getHeight(...) { return 0; }
	inline DisplayMode getDisplayMode(...) { return DisplayMode(); }
	inline DisplayMode getDesktopDisplayMode(...) { return DisplayMode(); }
	inline void* getAvailableDisplayModes(...) { return 0; }
}
#endif // DISPLAY_H
