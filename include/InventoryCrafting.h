#ifndef INVENTORYCRAFTING_H
#define INVENTORYCRAFTING_H

// Auto-generated from InventoryCrafting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "IInventory.h"
#include "ItemStack.h"
#include <string>

class GuiContainer;

class InventoryCrafting : public IInventory {
public:
	InventoryCrafting();
	InventoryCrafting(GuiContainer* var1, int var2, int var3);
	virtual ~InventoryCrafting();

	virtual int getSizeInventory();
	virtual ItemStack getStackInSlot(int var1);
	virtual const std::string& getInvName();
	virtual ItemStack decrStackSize(int var1, int var2);
	virtual void setInventorySlotContents(int var1, const ItemStack& var2);
	virtual int getInventoryStackLimit();

private:
	ItemStack* m_stackList;
	int inventoryWidth;
	GuiContainer* m_eventHandler;

};

#endif // INVENTORYCRAFTING_H