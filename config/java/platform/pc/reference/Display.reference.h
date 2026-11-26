// LWJGL org.lwjgl.opengl.Display on the PC: an SDL2 window with an OpenGL context. update() presents
// the frame and pumps SDL's events into the Keyboard/Mouse queues (Input.h).
#ifndef DISPLAY_H
#define DISPLAY_H
#include <string>
#include "DisplayMode.h"

namespace Display {
	void setDisplayMode(DisplayMode* mode);
	void setDisplayMode(const DisplayMode& mode);
	void create();
	template<class P, class C> inline void create(P*, C*) { create(); }   // Display.create(new PixelFormat(), new ContextAttribs(1, 2)): the context the layer makes
	template<class P> inline void create(P) { create(); }   // create(PixelFormat): SDL's depth/stencil
	void destroy();
	void update();                                      // present + pump input
	void swapBuffers();
	void processMessages();                             // pump input without presenting
	bool isCloseRequested();
	bool isActive();
	bool isCreated();
	inline bool isVisible() { return isCreated(); }
	int  getWidth();
	int  getHeight();
	DisplayMode getDisplayMode();
	DisplayMode getDesktopDisplayMode();
	void setTitle(const std::string& title);
	void setFullscreen(bool full);
	void setVSyncEnabled(bool on);
	void setParent(const void* parent);   // a game in a canvas (an applet's): closing the window ends the program, as the applet's frame did
	inline void setIcon(...) {}
	inline void setDisplayConfiguration(float, float, float) {}   // gamma, brightness, contrast: the screen keeps its own (Alpha 1.1's Minecraft)
	void sync(int fps);
	inline void setResizable(...) {}
	inline void setLocation(...) {}
	inline void releaseContext(...) {}
	inline void makeCurrent(...) {}
	inline bool isDirty(...) { return false; }
	inline void* getAvailableDisplayModes(...) { return 0; }
}
#endif // DISPLAY_H
