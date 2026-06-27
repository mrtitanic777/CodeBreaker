#ifndef INVENTORYCRAFTRESULT_H
#define INVENTORYCRAFTRESULT_H

// Auto-generated from InventoryCraftResult.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "IInventory.h"

class ItemStack;

class InventoryCraftResult : public IInventory {
public:
	InventoryCraftResult();
	virtual ~InventoryCraftResult();

	JArray<ItemStack* > stackResult;

	virtual int getSizeInventory();
	virtual ItemStack* getStackInSlot(int var1);
	virtual JavaString getInvName();
	virtual ItemStack* decrStackSize(int var1, int var2);
	virtual void setInventorySlotContents(int var1, ItemStack* var2);
	virtual int getInventoryStackLimit();
	virtual void onInventoryChanged();

	static void initStatics();
};

#endif // INVENTORYCRAFTRESULT_H