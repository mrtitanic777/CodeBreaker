#ifndef SLOT_H
#define SLOT_H

// Auto-generated from Slot.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class IInventory;
class ItemStack;

class Slot {
public:
	Slot(IInventory* var1, int var2);
	virtual ~Slot();

	int slotIndex;
	IInventory* inventory;

	virtual void onPickupFromSlot();
	virtual bool isItemValid(ItemStack* var1);
	virtual ItemStack* getStack();
	virtual void putStack(ItemStack* var1);
	virtual int getBackgroundIconIndex();
	virtual void onSlotChanged();
	virtual int getSlotStackLimit();

	static void initStatics();
};

#endif // SLOT_H