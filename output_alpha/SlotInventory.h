#ifndef SLOTINVENTORY_H
#define SLOTINVENTORY_H

// Auto-generated from SlotInventory.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Slot.h"

class GuiContainer;
class IInventory;

class SlotInventory : public Slot {
public:
	SlotInventory();
	SlotInventory(GuiContainer* var1, IInventory* var2, int var3, int var4, int var5);
	virtual ~SlotInventory();

	int m_xPos;
	int m_yPos;

	virtual bool isAtCursorPos(int var1, int var2);

public: // was private
	GuiContainer* m_guiHandler;

};

#endif // SLOTINVENTORY_H