#ifndef WINDOWADAPTER_H
#define WINDOWADAPTER_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for the external Java type WindowAdapter.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>

class WindowAdapter {
public:
	WindowAdapter(...) {}
	virtual ~WindowAdapter() {}
	virtual void windowClosing(...) {}
	virtual void windowOpened(...) {}
	virtual void windowClosed(...) {}
	virtual void windowActivated(...) {}
	virtual void windowDeactivated(...) {}
	virtual void windowIconified(...) {}
	virtual void windowDeiconified(...) {}
};
#endif // WINDOWADAPTER_H
