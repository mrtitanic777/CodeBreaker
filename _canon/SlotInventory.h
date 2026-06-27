#ifndef SLOTINVENTORY_H
#define SLOTINVENTORY_H

// Auto-generated from SlotInventory.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Slot.h"

class GuiContainer;
class IInventory;

class SlotInventory : public Slot {
public:
	SlotInventory(GuiContainer* var1, IInventory* var2, int var3, int var4, int var5);
	virtual ~SlotInventory();

	GuiContainer* guiHandler;
	int xPos;
	int yPos;

	virtual bool isAtCursorPos(int var1, int var2);

	static void initStatics();
};

#endif // SLOTINVENTORY_H