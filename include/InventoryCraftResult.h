#ifndef INVENTORYCRAFTRESULT_H
#define INVENTORYCRAFTRESULT_H

// Auto-generated from InventoryCraftResult.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "IInventory.h"
#include "ItemStack.h"
#include <string>

class InventoryCraftResult : public IInventory {
public:
	InventoryCraftResult();
	virtual ~InventoryCraftResult();

	virtual int getSizeInventory();
	virtual ItemStack getStackInSlot(int var1);
	virtual const std::string& getInvName();
	virtual ItemStack decrStackSize(int var1, int var2);
	virtual void setInventorySlotContents(int var1, const ItemStack& var2);
	virtual int getInventoryStackLimit();

private:
	ItemStack* m_stackResult;

};

#endif // INVENTORYCRAFTRESULT_H