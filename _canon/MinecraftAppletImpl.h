#ifndef MINECRAFTAPPLETIMPL_H
#define MINECRAFTAPPLETIMPL_H

// Auto-generated from MinecraftAppletImpl.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Minecraft.h"

class Canvas;
class Component;
class MinecraftApplet;
class UnexpectedThrowable;

class MinecraftAppletImpl : public Minecraft {
public:
	MinecraftAppletImpl(MinecraftApplet* var1, Component* var2, Canvas* var3, MinecraftApplet* var4, int var5, int var6, bool var7);
	virtual ~MinecraftAppletImpl();

	MinecraftApplet* mainFrame;

	virtual void displayUnexpectedThrowable(UnexpectedThrowable* var1);

	static void initStatics();
};

#endif // MINECRAFTAPPLETIMPL_H