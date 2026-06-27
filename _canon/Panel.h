#ifndef PANEL_H
#define PANEL_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for java.awt.Panel.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>
#include "Component.h"
class Graphics;

class Panel : public Component {
public:
	Panel(...) {}
	virtual ~Panel() {}
	virtual void setFocusable(...) {}
	virtual void setSize(...) {}
	virtual void repaint(...) {}
	virtual void requestFocus(...) {}
	virtual void setVisible(...) {}
	virtual void addNotify(...) {}
	virtual void removeNotify(...) {}
	virtual void setPreferredSize(...) {}
	virtual void setMinimumSize(...) {}
	virtual void setMaximumSize(...) {}
	virtual void enableInputMethods(...) {}
	virtual void setBackground(...) {}
	virtual void setForeground(...) {}
	virtual void validate(...) {}
	virtual void setLayout(...) {}
	virtual void add(...) {}
	virtual void remove(...) {}
	virtual void removeAll(...) {}
	virtual void addKeyListener(...) {}
	virtual void addMouseListener(...) {}
	virtual void addMouseMotionListener(...) {}
	virtual void addMouseWheelListener(...) {}
	virtual void addFocusListener(...) {}
	virtual void addComponentListener(...) {}
	virtual void addWindowListener(...) {}
	virtual void setFocusTraversalKeysEnabled(...) {}
	virtual void setCursor(...) {}
	virtual void createBufferStrategy(...) {}
	virtual void transferFocus(...) {}
	virtual void paint(...) {}
	virtual void update(...) {}
	virtual void doLayout(...) {}
	virtual void invalidate(...) {}
	virtual void revalidate(...) {}
	virtual void pack(...) {}
	virtual int getWidth(...) { return 0; }
	virtual int getHeight(...) { return 0; }
	virtual Graphics* getGraphics(...) { return 0; }
};
#endif // PANEL_H
