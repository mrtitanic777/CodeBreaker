#ifndef SLOTCRAFTING_H
#define SLOTCRAFTING_H

// Auto-generated from SlotCrafting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "SlotInventory.h"

class GuiContainer;
class IInventory;
class ItemStack;

class SlotCrafting : public SlotInventory {
public:
	SlotCrafting();
	SlotCrafting(GuiContainer* var1, IInventory* var2, IInventory* var3, int var4, int var5, int var6);
	virtual ~SlotCrafting();

	virtual bool isItemValid(ItemStack* var1);
	virtual void onPickupFromSlot();

public: // was private
	IInventory* m_craftMatrix;

};

#endif // SLOTCRAFTING_H