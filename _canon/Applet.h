#ifndef APPLET_H
#define APPLET_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for java.applet.Applet.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>
#include "Component.h"
class URL;
class Graphics;

class Applet : public Component {
public:
	Applet(...) {}
	virtual ~Applet() {}
	virtual void validate(...) {}
	virtual void setLayout(...) {}
	virtual void add(...) {}
	virtual void repaint(...) {}
	virtual void requestFocus(...) {}
	virtual void setVisible(...) {}
	virtual void resize(...) {}
	virtual void init(...) {}
	virtual void start(...) {}
	virtual void stop(...) {}
	virtual void remove(...) {}
	virtual void removeAll(...) {}
	virtual void setSize(...) {}
	virtual void setBackground(...) {}
	virtual void setForeground(...) {}
	virtual void addKeyListener(...) {}
	virtual void addMouseListener(...) {}
	virtual void addMouseMotionListener(...) {}
	virtual void addFocusListener(...) {}
	virtual void addComponentListener(...) {}
	virtual void setFocusable(...) {}
	virtual void setFocusTraversalKeysEnabled(...) {}
	virtual void enableInputMethods(...) {}
	virtual std::string getParameter(...) { return std::string(); }
	virtual URL* getDocumentBase(...) { return 0; }
	virtual URL* getCodeBase(...) { return 0; }
	virtual int getWidth(...) { return 0; }
	virtual int getHeight(...) { return 0; }
	virtual Graphics* getGraphics(...) { return 0; }
	virtual bool isActive(...) { return false; }
	virtual bool isDisplayable(...) { return false; }
	virtual void* getParent(...) { return 0; }
};
#endif // APPLET_H
