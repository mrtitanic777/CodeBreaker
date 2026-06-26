#ifndef SLOT_H
#define SLOT_H

// Auto-generated from Slot.java
// Target: Xbox 360 / C++03 / DirectX 9

class IInventory;
class ItemStack;

class Slot {
public:
	Slot();
	Slot(IInventory* var1, int var2);
	virtual ~Slot();

	int m_slotIndex;
	IInventory* m_inventory;

	virtual void onPickupFromSlot();
	virtual bool isItemValid(ItemStack* var1);
	virtual ItemStack* getStack();
	virtual void putStack(ItemStack* var1);
	virtual int func_775_c();
	virtual void onSlotChanged();
	virtual int getSlotStackLimit();

};

#endif // SLOT_H