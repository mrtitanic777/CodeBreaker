#ifndef CANVASMINECRAFTAPPLET_H
#define CANVASMINECRAFTAPPLET_H

// Auto-generated from CanvasMinecraftApplet.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Canvas.h"

class MinecraftApplet;

class CanvasMinecraftApplet : public Canvas {
public:
	CanvasMinecraftApplet(MinecraftApplet* var1);
	virtual ~CanvasMinecraftApplet();

	MinecraftApplet* mcApplet;

	virtual void addNotify();
	virtual void removeNotify();

	static void initStatics();
};

#endif // CANVASMINECRAFTAPPLET_H