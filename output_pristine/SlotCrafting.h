#ifndef SLOTCRAFTING_H
#define SLOTCRAFTING_H

// Auto-generated from SlotCrafting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Slot.h"

class GuiContainer;
class IInventory;

class SlotCrafting : public Slot {
public:
	SlotCrafting();
	SlotCrafting(GuiContainer* var1, IInventory* var2, IInventory* var3, int var4, int var5, int var6);
	virtual ~SlotCrafting();

	virtual bool isItemValid();
	virtual void onPickupFromSlot();

private:
	IInventory* m_craftMatrix;

};

#endif // SLOTCRAFTING_H