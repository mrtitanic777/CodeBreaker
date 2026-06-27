#ifndef MINECRAFTIMPL_H
#define MINECRAFTIMPL_H

// Auto-generated from MinecraftImpl.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Minecraft.h"

class Canvas;
class Component;
class Frame;
class MinecraftApplet;
class UnexpectedThrowable;

class MinecraftImpl : public Minecraft {
public:
	MinecraftImpl(Component* var1, Canvas* var2, MinecraftApplet* var3, int var4, int var5, bool var6, Frame* var7);
	virtual ~MinecraftImpl();

	Frame* mainFrame;

	virtual void displayUnexpectedThrowable(UnexpectedThrowable* var1);
};

#endif // MINECRAFTIMPL_H