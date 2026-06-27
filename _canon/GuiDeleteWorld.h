#ifndef GUIDELETEWORLD_H
#define GUIDELETEWORLD_H

// Auto-generated from GuiDeleteWorld.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "GuiSelectWorld.h"

class GuiScreen;

class GuiDeleteWorld : public GuiSelectWorld {
public:
	GuiDeleteWorld(GuiScreen* var1);
	virtual ~GuiDeleteWorld();

	virtual void initGui2();
	virtual void selectWorld(int var1);
	virtual void deleteWorld(bool var1, int var2);

	static void initStatics();
};

#endif // GUIDELETEWORLD_H