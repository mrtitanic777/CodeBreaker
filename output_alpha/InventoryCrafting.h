#ifndef INVENTORYCRAFTING_H
#define INVENTORYCRAFTING_H

// Auto-generated from InventoryCrafting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "IInventory.h"
#include <string>

class CraftingInventoryCB;
class ItemStack;

class InventoryCrafting : public IInventory {
public:
	InventoryCrafting();
	InventoryCrafting(CraftingInventoryCB* var1, int var2, int var3);
	InventoryCrafting(CraftingInventoryCB* var1, ItemStack** var2);
	virtual ~InventoryCrafting();

	virtual int getSizeInventory();
	virtual ItemStack* getStackInSlot(int var1);
	virtual const std::string& getInvName();
	virtual ItemStack* decrStackSize(int var1, int var2);
	virtual void setInventorySlotContents(int var1, ItemStack* var2);
	virtual int getInventoryStackLimit();
	virtual void onInventoryChanged();

public: // was private
	ItemStack** m_stackList;
	int m_nbrSlots;
	CraftingInventoryCB* m_eventHandler;

public:  // array-length companions
	int m_stackList_count;

};

#endif // INVENTORYCRAFTING_H