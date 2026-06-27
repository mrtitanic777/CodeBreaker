#ifndef MINECRAFTAPPLET_H
#define MINECRAFTAPPLET_H

// Auto-generated from MinecraftApplet.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Applet.h"

class Canvas;
class Minecraft;
class Thread;

class MinecraftApplet : public Applet {
public:
	virtual ~MinecraftApplet();

	Canvas* mcCanvas;
	Minecraft* mc;
	Thread* mcThread;

	virtual void init();
	virtual void startMainThread();
	virtual void start();
	virtual void stop();
	virtual void destroy();
	virtual void shutdown();
	virtual void clearReferences();
};

#endif // MINECRAFTAPPLET_H