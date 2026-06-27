#ifndef COMPONENTEVENT_H
#define COMPONENTEVENT_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for the external Java type ComponentEvent.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>

class ComponentEvent {
public:
	ComponentEvent(...) {}
	virtual ~ComponentEvent() {}
	virtual void consume(...) {}
	virtual int getKeyCode(...) { return 0; }
	virtual char getKeyChar(...) { return 0; }
	virtual int getX(...) { return 0; }
	virtual int getY(...) { return 0; }
	virtual int getButton(...) { return 0; }
	virtual int getClickCount(...) { return 0; }
	virtual int getWheelRotation(...) { return 0; }
	virtual void* getWindow(...) { return 0; }
	virtual void* getComponent(...) { return 0; }
	virtual bool isShiftDown(...) { return false; }
	virtual bool isControlDown(...) { return false; }
	virtual bool isAltDown(...) { return false; }
};
#endif // COMPONENTEVENT_H
