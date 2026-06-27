#ifndef INVENTORYLARGECHEST_H
#define INVENTORYLARGECHEST_H

// Auto-generated from InventoryLargeChest.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "IInventory.h"

class ItemStack;

class InventoryLargeChest : public IInventory {
public:
	InventoryLargeChest(const JavaString& var1, IInventory* var2, IInventory* var3);
	virtual ~InventoryLargeChest();

	JavaString name;
	IInventory* upperChest;
	IInventory* lowerChest;

	virtual int getSizeInventory();
	virtual JavaString getInvName();
	virtual ItemStack* getStackInSlot(int var1);
	virtual ItemStack* decrStackSize(int var1, int var2);
	virtual void setInventorySlotContents(int var1, ItemStack* var2);
	virtual int getInventoryStackLimit();
	virtual void onInventoryChanged();

	static void initStatics();
};

#endif // INVENTORYLARGECHEST_H