#ifndef INVENTORYCRAFTING_H
#define INVENTORYCRAFTING_H

// Auto-generated from InventoryCrafting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "IInventory.h"

class CraftingInventoryCB;
class ItemStack;

class InventoryCrafting : public IInventory {
public:
	InventoryCrafting(CraftingInventoryCB* var1, int var2, int var3);
	InventoryCrafting(CraftingInventoryCB* var1, JArray<ItemStack* > var2);
	virtual ~InventoryCrafting();

	JArray<ItemStack* > stackList;
	int nbrSlots;
	CraftingInventoryCB* eventHandler;

	virtual int getSizeInventory();
	virtual ItemStack* getStackInSlot(int var1);
	virtual JavaString getInvName();
	virtual ItemStack* decrStackSize(int var1, int var2);
	virtual void setInventorySlotContents(int var1, ItemStack* var2);
	virtual int getInventoryStackLimit();
	virtual void onInventoryChanged();

	static void initStatics();
};

#endif // INVENTORYCRAFTING_H